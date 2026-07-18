from __future__ import annotations

from dataclasses import dataclass
from typing import Sequence

import numpy as np

from app.domain.analysis import ClassStatistic
from app.domain.indices import SpectralIndices


VEGETATION_MIN = 0.20
MODERATE_VEGETATION_MIN = 0.40
DENSE_VEGETATION_MIN = 0.60
WATER_MNDWI_MIN = 0.10
WATER_NDWI_MIN = 0.05
BUILT_UP_NDBI_MIN = 0.05
BUILT_UP_UI_MIN = 0.00
BARE_SOIL_BSI_MIN = 0.00


@dataclass(frozen=True, slots=True)
class MapClass:
    class_id: int
    key: str
    name: str
    condition: str
    color: str


VEGETATION_CLASSES: tuple[MapClass, ...] = (
    MapClass(1, "no_vegetation", "Нет растительности", "NDVI < 0.20", "#8c8c8c"),
    MapClass(2, "sparse", "Слабая растительность", "0.20 ≤ NDVI < 0.40", "#d9ef8b"),
    MapClass(3, "moderate", "Средняя растительность", "0.40 ≤ NDVI < 0.60", "#66bd63"),
    MapClass(4, "dense", "Густая растительность", "NDVI ≥ 0.60", "#006837"),
)

LAND_COVER_CLASSES: tuple[MapClass, ...] = (
    MapClass(1, "water", "Вода", "MNDWI > 0.10, NDWI > 0.05", "#2b83ba"),
    MapClass(2, "bare_soil", "Открытая почва", "BSI > 0.00", "#d8b365"),
    MapClass(3, "built_up", "Застройка", "NDBI > 0.05, UI > 0.00", "#d73027"),
    MapClass(4, "sparse_vegetation", "Слабая растительность", "0.20 ≤ NDVI < 0.40", "#d9ef8b"),
    MapClass(5, "moderate_vegetation", "Средняя растительность", "0.40 ≤ NDVI < 0.60", "#66bd63"),
    MapClass(6, "dense_vegetation", "Густая растительность", "NDVI ≥ 0.60", "#006837"),
    MapClass(7, "other", "Прочая поверхность", "Остальные валидные пиксели", "#969696"),
)


def classify_ndvi(ndvi: np.ndarray, valid_mask: np.ndarray | None = None) -> np.ndarray:
    valid = np.isfinite(ndvi)
    if valid_mask is not None:
        valid &= valid_mask

    result = np.zeros(ndvi.shape, dtype="uint8")
    result[(ndvi < VEGETATION_MIN) & valid] = 1
    result[(ndvi >= VEGETATION_MIN) & (ndvi < MODERATE_VEGETATION_MIN) & valid] = 2
    result[
        (ndvi >= MODERATE_VEGETATION_MIN) & (ndvi < DENSE_VEGETATION_MIN) & valid
    ] = 3
    result[(ndvi >= DENSE_VEGETATION_MIN) & valid] = 4
    return result


def classify_land_cover(
    indices: SpectralIndices,
    valid_mask: np.ndarray | None = None,
) -> np.ndarray:
    valid = np.ones(indices.ndvi.shape, dtype=bool)
    for index in indices.as_dict().values():
        valid &= np.isfinite(index)
    if valid_mask is not None:
        valid &= valid_mask

    ndvi = indices.ndvi
    result = np.zeros(ndvi.shape, dtype="uint8")

    # Назначаем классы от низкого приоритета к высокому. Итоговый приоритет:
    # вода → растительность → застройка → почва → прочее.
    result[valid] = 7
    result[valid & (indices.bsi > BARE_SOIL_BSI_MIN)] = 2

    built_up = (
        valid
        & (ndvi < VEGETATION_MIN)
        & (indices.ndbi > BUILT_UP_NDBI_MIN)
        & (indices.urban_index > BUILT_UP_UI_MIN)
        & (indices.ndbi >= indices.bsi)
    )
    result[built_up] = 3

    result[valid & (ndvi >= VEGETATION_MIN) & (ndvi < MODERATE_VEGETATION_MIN)] = 4
    result[
        valid & (ndvi >= MODERATE_VEGETATION_MIN) & (ndvi < DENSE_VEGETATION_MIN)
    ] = 5
    result[valid & (ndvi >= DENSE_VEGETATION_MIN)] = 6

    water = (
        valid
        & (indices.mndwi > WATER_MNDWI_MIN)
        & (indices.ndwi > WATER_NDWI_MIN)
        & (ndvi < VEGETATION_MIN)
    )
    result[water] = 1
    return result


def calculate_statistics(
    classified: np.ndarray,
    classes: Sequence[MapClass],
) -> tuple[ClassStatistic, ...]:
    total = int(np.count_nonzero(classified))
    statistics = []
    for item in classes:
        count = int(np.count_nonzero(classified == item.class_id))
        statistics.append(
            ClassStatistic(
                class_id=item.class_id,
                key=item.key,
                name=item.name,
                condition=item.condition,
                color=item.color,
                pixel_count=count,
                percent=round(count / total * 100, 2) if total else 0.0,
            )
        )
    return tuple(statistics)
