from __future__ import annotations

from pathlib import Path

import numpy as np
import pandas as pd


class VegetationError(Exception):
    """Понятная ошибка проекта, которую можно показать пользователю."""


def _band_sort_key(path: Path) -> tuple[int, str]:
    """Помогает выбрать самый подходящий файл канала, если найдено несколько."""
    normalized = path.as_posix().lower()
    score = 0
    if "/img_data/r10m/" in normalized:
        score -= 30
    if "_10m." in normalized:
        score -= 20
    if normalized.endswith(".jp2"):
        score -= 10
    if normalized.endswith(".tif") or normalized.endswith(".tiff"):
        score -= 5
    return score, normalized


def _find_band_candidates(product_dir: Path, band: str) -> list[Path]:
    """Ищет файлы одного канала внутри продукта Sentinel-2."""
    patterns = [
        f"**/IMG_DATA/R10m/*_{band}_10m.jp2",
        f"**/IMG_DATA/R10m/*_{band}_10m.tif",
        f"**/IMG_DATA/R10m/*_{band}_10m.tiff",
        f"**/*_{band}_10m.jp2",
        f"**/*_{band}_10m.tif",
        f"**/*_{band}_10m.tiff",
        f"**/{band}.tif",
        f"**/{band}.tiff",
    ]

    candidates: list[Path] = []
    for pattern in patterns:
        candidates.extend(path for path in product_dir.glob(pattern) if path.is_file())

    unique_candidates = sorted(set(candidates), key=_band_sort_key)
    return unique_candidates


def find_bands_in_product(product_dir: Path) -> tuple[Path, Path]:
    """Находит каналы B04 и B08 в указанной папке продукта Sentinel-2."""
    if not product_dir.exists():
        raise VegetationError(f"Папка продукта не найдена: {product_dir}")
    if not product_dir.is_dir():
        raise VegetationError(f"Ожидалась папка продукта, но найден файл: {product_dir}")

    red_candidates = _find_band_candidates(product_dir, "B04")
    nir_candidates = _find_band_candidates(product_dir, "B08")

    if not red_candidates:
        raise VegetationError(f"В продукте не найден канал B04: {product_dir}")
    if not nir_candidates:
        raise VegetationError(f"В продукте не найден канал B08: {product_dir}")

    return red_candidates[0], nir_candidates[0]


def require_rasterio():
    try:
        import rasterio
    except ImportError as error:
        raise VegetationError(
            "Не установлена библиотека rasterio. "
            "Установите зависимости командой: python -m pip install -r requirements.txt"
        ) from error
    return rasterio


def find_default_bands(data_dir: Path) -> tuple[Path, Path]:
    """Находит каналы B04 и B08 в папке data."""
    l2a_products = sorted(data_dir.glob("L2A_*"))
    for product_dir in l2a_products:
        try:
            return find_bands_in_product(product_dir)
        except VegetationError:
            continue

    raw_red = data_dir / "raw" / "B04.tif"
    raw_nir = data_dir / "raw" / "B08.tif"
    if raw_red.exists() and raw_nir.exists():
        return raw_red, raw_nir

    raise VegetationError(
        "Не найдены входные каналы. Положите B04.tif и B08.tif в data/raw "
        "или добавьте продукт Sentinel-2 L2A в папку data."
    )


def load_bands(red_path: Path, nir_path: Path) -> tuple[np.ndarray, np.ndarray, np.ndarray, dict]:
    """Загружает два канала Sentinel-2: B04 и B08."""
    rasterio = require_rasterio()

    if not red_path.exists():
        raise VegetationError(f"Файл красного канала не найден: {red_path}")
    if not nir_path.exists():
        raise VegetationError(f"Файл ближнего инфракрасного канала не найден: {nir_path}")

    try:
        with rasterio.open(red_path) as red_dataset, rasterio.open(nir_path) as nir_dataset:
            if red_dataset.shape != nir_dataset.shape:
                raise VegetationError(
                    "Каналы B04 и B08 имеют разные размеры: "
                    f"{red_dataset.shape} и {nir_dataset.shape}"
                )

            red = red_dataset.read(1, masked=True).astype("float32")
            nir = nir_dataset.read(1, masked=True).astype("float32")
            profile = red_dataset.profile.copy()
    except VegetationError:
        raise
    except Exception as error:
        raise VegetationError(f"Не удалось прочитать GeoTIFF-файлы: {error}") from error

    red_array = np.asarray(red.filled(np.nan), dtype="float32")
    nir_array = np.asarray(nir.filled(np.nan), dtype="float32")

    red_mask = np.ma.getmaskarray(red)
    nir_mask = np.ma.getmaskarray(nir)
    valid_mask = (~red_mask) & (~nir_mask)
    valid_mask &= np.isfinite(red_array) & np.isfinite(nir_array)

    return red_array, nir_array, valid_mask, profile


def save_geotiff(path: Path, array: np.ndarray, base_profile: dict, dtype: str, nodata: float | int) -> None:
    """Сохраняет расчетный массив как одноканальный GeoTIFF."""
    rasterio = require_rasterio()
    path.parent.mkdir(parents=True, exist_ok=True)

    output = array.copy()
    if dtype.startswith("float"):
        output = np.where(np.isfinite(output), output, nodata).astype(dtype)
    else:
        output = output.astype(dtype)

    profile = base_profile.copy()
    profile.update(
        {
            "driver": "GTiff",
            "count": 1,
            "dtype": dtype,
            "nodata": nodata,
            "compress": "lzw",
        }
    )

    with rasterio.open(path, "w", **profile) as dataset:
        dataset.write(output, 1)


def make_summary_text(red_path: Path, nir_path: Path, statistics: pd.DataFrame) -> str:
    """Формирует текст отчета с процентами классов."""
    lines = [
        "Краткий отчет по классификации растительности",
        "",
        f"Красный канал B04: {red_path}",
        f"Ближний инфракрасный канал B08: {nir_path}",
        "",
        "Метод: NDVI = (B08 - B04) / (B08 + B04)",
        "",
        "Распределение классов:",
    ]

    for row in statistics.to_dict("records"):
        lines.append(
            f"- Класс {row['class_id']}: {row['class_name']} "
            f"({row['ndvi_range']}) - {row['pixel_count']} пикс., {row['percent']}%"
        )

    return "\n".join(lines) + "\n"


def write_summary(summary_path: Path, red_path: Path, nir_path: Path, statistics: pd.DataFrame) -> str:
    """Сохраняет короткий текстовый отчет и возвращает его текст."""
    summary_path.parent.mkdir(parents=True, exist_ok=True)

    summary_text = make_summary_text(red_path, nir_path, statistics)
    summary_path.write_text(summary_text, encoding="utf-8")
    return summary_text
