from __future__ import annotations

import re
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

from src.errors import VegetationError
from src.satellite.calibration import RadiometricCalibration, discover_l2a_calibration


SPECTRAL_BAND_CODES = ("B02", "B03", "B04", "B08", "B11", "B12")
SPECTRAL_FIELD_CODES = {
    "blue": "B02",
    "green": "B03",
    "red": "B04",
    "nir": "B08",
    "swir1": "B11",
    "swir2": "B12",
}
SUPPORTED_RASTERS = {".jp2", ".tif", ".tiff"}

NDVI_RESOLUTION = {code: "10m" for code in SPECTRAL_BAND_CODES}
FULL_RESOLUTION = {
    "B02": "20m",
    "B03": "20m",
    "B04": "20m",
    "B08": "10m",
    "B11": "20m",
    "B12": "20m",
    "SCL": "20m",
}


@dataclass(frozen=True)
class SentinelBandPaths:
    """Пути к каналам одной сцены Sentinel-2."""

    red: Path
    nir: Path
    blue: Path | None = None
    green: Path | None = None
    swir1: Path | None = None
    swir2: Path | None = None
    scene_classification: Path | None = None
    calibration: RadiometricCalibration | None = None

    @property
    def is_multispectral(self) -> bool:
        return all((self.blue, self.green, self.swir1, self.swir2))

    def spectral_paths(self) -> dict[str, Path]:
        values = {
            "blue": self.blue,
            "green": self.green,
            "red": self.red,
            "nir": self.nir,
            "swir1": self.swir1,
            "swir2": self.swir2,
        }
        return {name: path for name, path in values.items() if path is not None}


def band_code_from_name(filename: str) -> str | None:
    """Вернуть код канала из имени файла, например T_..._B04_10m.jp2."""

    name = filename.replace("\\", "/").rsplit("/", 1)[-1]
    path = Path(name)
    if path.suffix.lower() not in SUPPORTED_RASTERS:
        return None

    stem = path.stem.upper()
    if stem.startswith(("MSK_", "QI_")):
        return None
    for code in (*SPECTRAL_BAND_CODES, "SCL"):
        if stem == code or re.search(
            rf"_{code}(?:_(?:10m|20m|60m))?$",
            stem,
            flags=re.IGNORECASE,
        ):
            return code
    return None


def _sort_key(path_text: str, band_code: str, preferred: str) -> tuple[int, int, str]:
    normalized = path_text.replace("\\", "/").casefold()
    return (
        0 if f"_{preferred}." in normalized else 1,
        0 if "/img_data/" in normalized else 1,
        normalized,
    )


def find_band_candidates(
    product_dir: Path,
    band_code: str,
    *,
    preferred_resolution: str | None = None,
) -> list[Path]:
    """Найти все варианты одного канала внутри папки продукта."""

    preferred = preferred_resolution or NDVI_RESOLUTION.get(band_code, "20m")
    candidates = [
        path
        for path in product_dir.rglob("*")
        if path.is_file() and band_code_from_name(path.name) == band_code
    ]
    return sorted(candidates, key=lambda path: _sort_key(str(path), band_code, preferred))


def _scene_key(path: Path, product_dir: Path) -> str:
    relative = path.relative_to(product_dir)
    for index, part in enumerate(relative.parts):
        if part.casefold() == "img_data":
            return "/".join(relative.parts[:index]).casefold()
    return relative.parent.as_posix().casefold()


def discover_sentinel_bands(
    product_dir: Path,
    *,
    require_multispectral: bool = False,
) -> SentinelBandPaths:
    """Найти B02/B03/B04/B08/B11/B12 в распакованном продукте."""

    if not product_dir.is_dir():
        raise VegetationError(f"Папка продукта не найдена: {product_dir}")

    grouped: dict[str, dict[str, list[Path]]] = defaultdict(lambda: defaultdict(list))
    for path in product_dir.rglob("*"):
        if not path.is_file():
            continue
        code = band_code_from_name(path.name)
        if code is not None:
            grouped[_scene_key(path, product_dir)][code].append(path)

    complete = [
        key for key, bands in grouped.items() if all(code in bands for code in SPECTRAL_BAND_CODES)
    ]
    pairs = [key for key, bands in grouped.items() if "B04" in bands and "B08" in bands]
    choices = complete or ([] if require_multispectral else pairs)

    if not choices:
        required = SPECTRAL_BAND_CODES if require_multispectral else ("B04", "B08")
        found = {code for bands in grouped.values() for code in bands}
        missing = ", ".join(code for code in required if code not in found)
        raise VegetationError(f"Не найдены обязательные каналы: {missing or 'одна полная сцена'}.")
    if len(choices) > 1:
        raise VegetationError("Найдено несколько гранул. Укажите папку одной сцены.")

    available = grouped[choices[0]]
    is_full = all(code in available for code in SPECTRAL_BAND_CODES)
    resolutions = FULL_RESOLUTION if is_full else NDVI_RESOLUTION

    selected: dict[str, Path] = {}
    for code, candidates in available.items():
        if code in (*SPECTRAL_BAND_CODES, "SCL"):
            preferred = resolutions.get(code, "20m")
            selected[code] = min(
                candidates,
                key=lambda path: _sort_key(str(path), code, preferred),
            )

    return SentinelBandPaths(
        blue=selected.get("B02"),
        green=selected.get("B03"),
        red=selected["B04"],
        nir=selected["B08"],
        swir1=selected.get("B11"),
        swir2=selected.get("B12"),
        scene_classification=selected.get("SCL"),
        calibration=discover_l2a_calibration(product_dir),
    )


def discover_default_bands(data_dir: Path) -> SentinelBandPaths:
    """Сначала искать SAFE-папку, затем отдельные файлы в data/raw."""

    if data_dir.exists():
        for product_dir in sorted(data_dir.iterdir()):
            if not product_dir.is_dir() or product_dir.name.casefold() in {"raw", "processed"}:
                continue
            try:
                return discover_sentinel_bands(product_dir)
            except VegetationError:
                pass

    raw_dir = data_dir / "raw"
    red = raw_dir / "B04.tif"
    nir = raw_dir / "B08.tif"
    if not red.exists() or not nir.exists():
        raise VegetationError(
            "Добавьте продукт Sentinel-2 в data или B04.tif и B08.tif в data/raw."
        )

    optional = {code: raw_dir / f"{code}.tif" for code in ("B02", "B03", "B11", "B12")}
    scl = raw_dir / "SCL.tif"
    return SentinelBandPaths(
        red=red,
        nir=nir,
        blue=optional["B02"] if optional["B02"].exists() else None,
        green=optional["B03"] if optional["B03"].exists() else None,
        swir1=optional["B11"] if optional["B11"].exists() else None,
        swir2=optional["B12"] if optional["B12"].exists() else None,
        scene_classification=scl if scl.exists() else None,
        calibration=discover_l2a_calibration(raw_dir),
    )
