from __future__ import annotations

import shutil
import zipfile
from pathlib import Path
from typing import BinaryIO

from app.domain.errors import VegetationError
from app.infrastructure.satellite.bands import (
    REQUIRED_BAND_CODES,
    SentinelBandPaths,
)


MAX_ARCHIVE_FILES = 20_000
MAX_BAND_SIZE = 512 * 1024**2


def _select_bands(members: list[zipfile.ZipInfo]) -> dict[str, zipfile.ZipInfo]:
    selected: dict[str, zipfile.ZipInfo] = {}

    for code in REQUIRED_BAND_CODES:
        if code in {"B02", "B03", "B04", "B08"}:
            suffix = f"_{code}_10M"
            preferred_dir = "/IMG_DATA/R10M/"
        else:
            suffix = f"_{code}_20M"
            preferred_dir = "/IMG_DATA/R20M/"

        candidates = []

        for member in members:
            path = member.filename.replace("\\", "/").upper()

            if (
                preferred_dir in path
                and path.endswith(".JP2")
                and suffix in path
            ):
                candidates.append(member)

        if not candidates:
            raise VegetationError(f"В ZIP не найден канал {code}.")
        selected[code] = candidates[0]

    return selected


def extract_sentinel_bands(
    source: Path | BinaryIO,
    destination: Path,
) -> SentinelBandPaths:
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
