from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Any

import numpy as np
import rasterio

from app.domain.errors import VegetationError
from app.infrastructure.satellite.bands import SentinelBandPaths


@dataclass(frozen=True, slots=True)
class RasterGrid:
    shape: tuple[int, int]
    crs: Any
    transform: Any


@dataclass(frozen=True, slots=True)
class LoadedRasterBands:
    red: np.ndarray
    nir: np.ndarray
    valid_mask: np.ndarray
    blue: np.ndarray | None = None
    green: np.ndarray | None = None
    swir1: np.ndarray | None = None
    swir2: np.ndarray | None = None

    @property
    def is_multispectral(self) -> bool:
        return all(
            band is not None for band in (self.blue, self.green, self.swir1, self.swir2)
        )


def _read_band(path: Path) -> tuple[np.ndarray, RasterGrid]:
    try:
        with rasterio.open(path) as dataset:
            data = dataset.read(1, out_dtype="float32")
            data[dataset.read_masks(1) == 0] = np.nan
            grid = RasterGrid(data.shape, dataset.crs, dataset.transform)
            return data, grid
    except Exception as error:
        raise VegetationError(f"Не удалось прочитать {path.name}: {error}") from error


def _same_grid(first: RasterGrid, second: RasterGrid) -> bool:
    return (
        first.shape == second.shape
        and first.crs == second.crs
        and first.transform.almost_equals(second.transform)
    )


def load_raster_bands(paths: SentinelBandPaths) -> LoadedRasterBands:
    arrays: dict[str, np.ndarray] = {}
    reference_grid: RasterGrid | None = None

    for name, path in paths.as_dict().items():
        array, grid = _read_band(path)
        if reference_grid is None:
            reference_grid = grid
        elif not _same_grid(reference_grid, grid):
            raise VegetationError(
                "Все каналы должны иметь одинаковые размер, CRS и пиксельную сетку. "
                f"Канал {path.name} не совпадает с первым каналом."
            )
        arrays[name] = array

    valid_mask = np.ones(next(iter(arrays.values())).shape, dtype=bool)
    for array in arrays.values():
        valid_mask &= np.isfinite(array)

    return LoadedRasterBands(
        blue=arrays.get("blue"),
        green=arrays.get("green"),
        red=arrays["red"],
        nir=arrays["nir"],
        swir1=arrays.get("swir1"),
        swir2=arrays.get("swir2"),
        valid_mask=valid_mask,
    )
