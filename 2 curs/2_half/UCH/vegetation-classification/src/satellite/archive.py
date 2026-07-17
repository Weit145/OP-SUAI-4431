from __future__ import annotations

import shutil
import zipfile
from collections import defaultdict
from pathlib import Path, PurePosixPath
from typing import BinaryIO

from src.errors import VegetationError
from src.satellite.bands import (
    FULL_RESOLUTION,
    SPECTRAL_BAND_CODES,
    SentinelBandPaths,
    band_code_from_name,
)
from src.satellite.calibration import discover_l2a_calibration


MAX_ARCHIVE_FILES = 20_000
MAX_SELECTED_SIZE = 2 * 1024**3
MAX_MEMBER_SIZE = 512 * 1024**2
MAX_COMPRESSION_RATIO = 200


def _scene_key(member: zipfile.ZipInfo) -> str:
    parts = PurePosixPath(member.filename.replace("\\", "/")).parts
    for index, part in enumerate(parts):
        if part.casefold() == "img_data":
            return "/".join(parts[:index]).casefold()
    return "/".join(parts[:-1]).casefold()


def _member_sort_key(member: zipfile.ZipInfo, code: str) -> tuple[int, str]:
    name = member.filename.replace("\\", "/").casefold()
    preferred = FULL_RESOLUTION[code]
    return (0 if f"_{preferred}." in name else 1, name)


def _select_members(members: list[zipfile.ZipInfo]) -> dict[str, zipfile.ZipInfo]:
    grouped: dict[str, dict[str, list[zipfile.ZipInfo]]] = defaultdict(lambda: defaultdict(list))
    for member in members:
        code = band_code_from_name(member.filename)
        if code is not None:
            grouped[_scene_key(member)][code].append(member)

    complete = [
        key for key, bands in grouped.items() if all(code in bands for code in SPECTRAL_BAND_CODES)
    ]
    if not complete:
        found = {code for bands in grouped.values() for code in bands}
        missing = ", ".join(code for code in SPECTRAL_BAND_CODES if code not in found)
        raise VegetationError(f"В ZIP не хватает каналов: {missing or 'одна полная сцена'}.")
    if len(complete) > 1:
        raise VegetationError("В ZIP найдено несколько гранул. Оставьте одну сцену.")

    candidates = grouped[complete[0]]
    selected = {
        code: min(candidates[code], key=lambda member: _member_sort_key(member, code))
        for code in SPECTRAL_BAND_CODES
    }
    if "SCL" in candidates:
        selected["SCL"] = min(
            candidates["SCL"],
            key=lambda member: _member_sort_key(member, "SCL"),
        )
    return selected


def _check_archive(members: list[zipfile.ZipInfo], selected: dict[str, zipfile.ZipInfo]) -> None:
    """Оставить только короткую защиту от случайно огромных ZIP."""

    if len(members) > MAX_ARCHIVE_FILES:
        raise VegetationError("В ZIP слишком много файлов.")
    if sum(member.file_size for member in selected.values()) > MAX_SELECTED_SIZE:
        raise VegetationError("Выбранные каналы занимают больше 2 ГБ.")
    for member in selected.values():
        ratio = member.file_size / max(member.compress_size, 1)
        if member.file_size > MAX_MEMBER_SIZE or ratio > MAX_COMPRESSION_RATIO:
            raise VegetationError(f"Слишком большой сжатый файл: {member.filename}")


def _copy_member(
    archive: zipfile.ZipFile,
    member: zipfile.ZipInfo,
    destination: Path,
) -> None:
    # Не используем extract/extractall: имя внутри ZIP никогда не становится
    # путём на диске, поэтому ../ не может выйти из временной папки.
    with archive.open(member) as source, destination.open("wb") as target:
        shutil.copyfileobj(source, target, length=1024 * 1024)


def extract_sentinel_bands(
    source: Path | BinaryIO,
    destination: Path,
) -> SentinelBandPaths:
    """Извлечь только нужные каналы одной сцены Sentinel-2."""

    destination.mkdir(parents=True, exist_ok=True)
    try:
        with zipfile.ZipFile(source) as archive:
            members = [member for member in archive.infolist() if not member.is_dir()]
            selected = _select_members(members)
            _check_archive(members, selected)

            extracted: dict[str, Path] = {}
            for code, member in selected.items():
                target = destination / f"{code}{Path(member.filename).suffix.lower()}"
                _copy_member(archive, member, target)
                extracted[code] = target

            # Для калибровки достаточно первого XML каждого стандартного типа.
            metadata_names = ("MTD_MSIL2A.xml", "MTD_DS.xml", "MTD_TL.xml")
            for expected_name in metadata_names:
                member = next(
                    (
                        item
                        for item in members
                        if PurePosixPath(item.filename.replace("\\", "/")).name.casefold()
                        == expected_name.casefold()
                    ),
                    None,
                )
                if member is not None and member.file_size <= 16 * 1024**2:
                    _copy_member(archive, member, destination / expected_name)
    except VegetationError:
        raise
    except zipfile.BadZipFile as error:
        raise VegetationError("Загруженный файл не является корректным ZIP.") from error
    except (OSError, RuntimeError) as error:
        raise VegetationError(f"Не удалось прочитать ZIP: {error}") from error

    return SentinelBandPaths(
        blue=extracted["B02"],
        green=extracted["B03"],
        red=extracted["B04"],
        nir=extracted["B08"],
        swir1=extracted["B11"],
        swir2=extracted["B12"],
        scene_classification=extracted.get("SCL"),
        calibration=discover_l2a_calibration(destination),
    )
