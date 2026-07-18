from __future__ import annotations

import re
import shutil
import zipfile
from pathlib import Path, PurePosixPath

from app.domain.errors import VegetationError
from app.infrastructure.satellite.bands import (
    REQUIRED_BAND_CODES,
    SUPPORTED_RASTERS,
    SentinelBandPaths,
)


MAX_ARCHIVE_FILES = 20_000
MAX_BAND_SIZE = 512 * 1024**2


def _band_code(filename: str) -> str | None:
    name = PurePosixPath(filename.replace("\\", "/")).name
    if Path(name).suffix.lower() not in SUPPORTED_RASTERS:
        return None

    stem = Path(name).stem.upper()
    for code in REQUIRED_BAND_CODES:
        if stem == code or re.search(rf"(?:^|_){code}(?:_|$)", stem):
            return code
    return None


def _select_bands(members: list[zipfile.ZipInfo]) -> dict[str, zipfile.ZipInfo]:
    candidates: dict[str, list[zipfile.ZipInfo]] = {
        code: [] for code in REQUIRED_BAND_CODES
    }
    for member in members:
        code = _band_code(member.filename)
        if code is not None:
            candidates[code].append(member)

    missing = [code for code, items in candidates.items() if not items]
    if missing:
        raise VegetationError("В ZIP не хватает каналов: " + ", ".join(missing) + ".")

    duplicates = [code for code, items in candidates.items() if len(items) > 1]
    if duplicates:
        raise VegetationError(
            "В ZIP должно быть ровно по одному файлу каждого канала. "
            "Найдены дубликаты: " + ", ".join(duplicates) + "."
        )
    return {code: items[0] for code, items in candidates.items()}


def extract_sentinel_bands(source: Path, destination: Path) -> SentinelBandPaths:
    destination.mkdir(parents=True, exist_ok=True)
    try:
        with zipfile.ZipFile(source) as archive:
            members = [member for member in archive.infolist() if not member.is_dir()]
            if len(members) > MAX_ARCHIVE_FILES:
                raise VegetationError("В ZIP слишком много файлов.")

            selected = _select_bands(members)
            extracted: dict[str, Path] = {}
            for code, member in selected.items():
                if member.file_size > MAX_BAND_SIZE:
                    raise VegetationError(f"Канал {code} занимает больше 512 МБ.")
                target = destination / f"{code}{Path(member.filename).suffix.lower()}"
                with archive.open(member) as input_file, target.open("wb") as output_file:
                    shutil.copyfileobj(input_file, output_file, length=1024 * 1024)
                extracted[code] = target
    except VegetationError:
        raise
    except zipfile.BadZipFile as error:
        raise VegetationError("Загруженный файл не является корректным ZIP.") from error
    except OSError as error:
        raise VegetationError(f"Не удалось прочитать ZIP: {error}") from error

    return SentinelBandPaths(
        blue=extracted["B02"],
        green=extracted["B03"],
        red=extracted["B04"],
        nir=extracted["B08"],
        swir1=extracted["B11"],
        swir2=extracted["B12"],
    )
