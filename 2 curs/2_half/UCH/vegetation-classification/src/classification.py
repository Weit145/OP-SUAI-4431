from __future__ import annotations

from dataclasses import dataclass

import numpy as np
import pandas as pd


@dataclass(frozen=True)
class VegetationClass:
    class_id: int
    name: str
    ndvi_range: str
    color: str


VEGETATION_CLASSES: tuple[VegetationClass, ...] = (
    VegetationClass(1, "Нет растительности / вода / застройка", "NDVI < 0.2", "#8c8c8c"),
    VegetationClass(2, "Слабая растительность", "0.2 <= NDVI < 0.4", "#d9ef8b"),
    VegetationClass(3, "Средняя растительность", "0.4 <= NDVI < 0.6", "#66bd63"),
    VegetationClass(4, "Густая растительность", "NDVI >= 0.6", "#006837"),
)


def classify_ndvi(ndvi: np.ndarray, valid_mask: np.ndarray | None = None) -> np.ndarray:
    """Классифицирует каждый валидный пиксель по диапазонам NDVI."""
    valid = np.isfinite(ndvi)
    if valid_mask is not None:
        valid &= valid_mask

    classes = np.zeros(ndvi.shape, dtype="uint8")
    classes[(ndvi < 0.2) & valid] = 1
    classes[(ndvi >= 0.2) & (ndvi < 0.4) & valid] = 2
    classes[(ndvi >= 0.4) & (ndvi < 0.6) & valid] = 3
    classes[(ndvi >= 0.6) & valid] = 4
    return classes


def calculate_statistics(classified: np.ndarray) -> pd.DataFrame:
    """Считает количество пикселей и процент площади для каждого класса."""
    valid_pixels = classified > 0
    total = int(np.count_nonzero(valid_pixels))

    rows: list[dict[str, object]] = []
    for item in VEGETATION_CLASSES:
        count = int(np.count_nonzero(classified == item.class_id))
        percent = round((count / total * 100.0), 2) if total else 0.0
        rows.append(
            {
                "class_id": item.class_id,
                "class_name": item.name,
                "ndvi_range": item.ndvi_range,
                "pixel_count": count,
                "percent": percent,
            }
        )

    return pd.DataFrame(rows)
