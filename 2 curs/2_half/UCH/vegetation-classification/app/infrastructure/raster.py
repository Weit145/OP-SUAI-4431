from __future__ import annotations

from dataclasses import dataclass
import math
from pathlib import Path
from typing import Any

import numpy as np
import rasterio
from rasterio.enums import Resampling
from rasterio.vrt import WarpedVRT

from app.domain.errors import VegetationError
from app.infrastructure.satellite.bands import SentinelBandPaths


MAX_ANALYSIS_PIXELS = 4_000_000


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


def _read_reference_band(path: Path) -> tuple[np.ndarray, RasterGrid]:
    try:
        with rasterio.open(path) as dataset:
            scale = max(
                1.0,
                math.sqrt(dataset.width * dataset.height / MAX_ANALYSIS_PIXELS),
            )
            height = max(1, math.ceil(dataset.height / scale))
            width = max(1, math.ceil(dataset.width / scale))
            shape = (height, width)
            data = dataset.read(
                1,
                out_shape=shape,
                out_dtype="float32",
                resampling=Resampling.bilinear,
            )
            mask = dataset.read_masks(
                1,
                out_shape=shape,
                resampling=Resampling.nearest,
            )
            data[mask == 0] = np.nan
            transform = dataset.transform * dataset.transform.scale(
                dataset.width / width,
                dataset.height / height,
            )
            grid = RasterGrid(shape, dataset.crs, transform)
            return data, grid
    except Exception as error:
        raise VegetationError(f"Не удалось прочитать {path.name}: {error}") from error


def _read_on_grid(path: Path, target_grid: RasterGrid) -> np.ndarray:
    try:
        with rasterio.open(path) as dataset:
            if dataset.crs is None or target_grid.crs is None:
                raise VegetationError(
                    f"У канала {path.name} отсутствует система координат."
                )
            with WarpedVRT(
                dataset,
                crs=target_grid.crs,
                transform=target_grid.transform,
                width=target_grid.shape[1],
                height=target_grid.shape[0],
                resampling=Resampling.bilinear,
                nodata=np.nan,
                dtype="float32",
            ) as warped:
                data = warped.read(1, out_dtype="float32")
                data[warped.read_masks(1) == 0] = np.nan
                return data
    except VegetationError:
        raise
    except Exception as error:
        raise VegetationError(f"Не удалось прочитать {path.name}: {error}") from error


def load_raster_bands(paths: SentinelBandPaths) -> LoadedRasterBands:
    arrays: dict[str, np.ndarray] = {}
    reference_grid: RasterGrid | None = None

    for name, path in paths.as_dict().items():
        if reference_grid is None:
            array, reference_grid = _read_reference_band(path)
            arrays[name] = array
            continue

        arrays[name] = _read_on_grid(path, reference_grid)

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
