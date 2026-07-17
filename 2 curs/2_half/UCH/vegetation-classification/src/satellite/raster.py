from __future__ import annotations

import warnings
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import numpy as np
import rasterio
from rasterio.enums import Resampling
from rasterio.warp import reproject

from src.errors import VegetationError
from src.satellite.bands import SPECTRAL_FIELD_CODES, SentinelBandPaths


@dataclass(frozen=True)
class RasterGrid:
    width: int
    height: int
    transform: Any
    crs: Any


@dataclass(frozen=True)
class LoadedRasterBands:
    red: np.ndarray
    nir: np.ndarray
    valid_mask: np.ndarray
    profile: dict[str, Any]
    blue: np.ndarray | None = None
    green: np.ndarray | None = None
    swir1: np.ndarray | None = None
    swir2: np.ndarray | None = None

    @property
    def is_multispectral(self) -> bool:
        return all(
            (
                self.blue is not None,
                self.green is not None,
                self.swir1 is not None,
                self.swir2 is not None,
            )
        )


def _grid(dataset: Any) -> RasterGrid:
    return RasterGrid(dataset.width, dataset.height, dataset.transform, dataset.crs)


def _same_grid(dataset: Any, grid: RasterGrid) -> bool:
    return (
        dataset.width == grid.width
        and dataset.height == grid.height
        and dataset.crs == grid.crs
        and dataset.transform.almost_equals(grid.transform)
    )


def _read_band(
    path: Path,
    grid: RasterGrid,
    *,
    categorical: bool = False,
    calibration: tuple[float, float] | None = None,
) -> np.ndarray:
    try:
        with rasterio.open(path) as dataset:
            with warnings.catch_warnings():
                warnings.filterwarnings("ignore", category=DeprecationWarning)
                source = dataset.read(1, out_dtype="float32")
                source[dataset.read_masks(1) == 0] = np.nan

            if not categorical:
                zero_dn = source == 0 if calibration is not None else None
                scale, offset = calibration or (
                    float(dataset.scales[0]),
                    float(dataset.offsets[0]),
                )
                source = source * scale + offset
                if zero_dn is not None:
                    source[zero_dn] = np.nan

            if _same_grid(dataset, grid):
                return source
            if dataset.crs is None or grid.crs is None:
                raise VegetationError(f"Для совмещения каналов нужен CRS: {path.name}")

            result = np.full((grid.height, grid.width), np.nan, dtype="float32")
            target_is_coarser = abs(grid.transform.a) > abs(dataset.transform.a)
            resampling = (
                Resampling.nearest
                if categorical
                else Resampling.average
                if target_is_coarser
                else Resampling.bilinear
            )
            reproject(
                source=source,
                destination=result,
                src_transform=dataset.transform,
                src_crs=dataset.crs,
                src_nodata=np.nan,
                dst_transform=grid.transform,
                dst_crs=grid.crs,
                dst_nodata=np.nan,
                resampling=resampling,
            )
            return result
    except VegetationError:
        raise
    except Exception as error:
        raise VegetationError(f"Не удалось прочитать {path.name}: {error}") from error


def load_raster_bands(paths: SentinelBandPaths) -> LoadedRasterBands:
    """Прочитать каналы и привести их к одной пиксельной сетке."""

    reference_path = paths.swir1 if paths.is_multispectral else paths.red
    assert reference_path is not None
    try:
        with rasterio.open(reference_path) as reference:
            grid = _grid(reference)
            profile = reference.profile.copy()
    except Exception as error:
        raise VegetationError(f"Не удалось открыть {reference_path.name}: {error}") from error

    arrays = {}
    for name, path in paths.spectral_paths().items():
        calibration = (
            paths.calibration.coefficients(SPECTRAL_FIELD_CODES[name])
            if paths.calibration is not None
            else None
        )
        arrays[name] = _read_band(path, grid, calibration=calibration)

    valid = np.logical_and.reduce([np.isfinite(array) for array in arrays.values()])
    valid &= np.logical_or.reduce([np.isfinite(array) & (array != 0) for array in arrays.values()])

    if paths.scene_classification is not None:
        scl = _read_band(paths.scene_classification, grid, categorical=True)
        scl_codes = np.where(np.isfinite(scl), np.rint(scl), -1).astype("int16")
        valid &= np.isfinite(scl) & ~np.isin(scl_codes, (0, 1, 2, 3, 8, 9, 10, 11))

    profile.update(
        driver="GTiff",
        height=grid.height,
        width=grid.width,
        count=1,
        transform=grid.transform,
        crs=grid.crs,
    )
    return LoadedRasterBands(
        blue=arrays.get("blue"),
        green=arrays.get("green"),
        red=arrays["red"],
        nir=arrays["nir"],
        swir1=arrays.get("swir1"),
        swir2=arrays.get("swir2"),
        valid_mask=valid,
        profile=profile,
    )


def save_geotiff(
    path: Path,
    array: np.ndarray,
    base_profile: dict[str, Any],
    *,
    dtype: str,
    nodata: float | int,
) -> None:
    """Сохранить одноканальный GeoTIFF с геопривязкой исходного снимка."""

    path.parent.mkdir(parents=True, exist_ok=True)
    output = np.where(np.isfinite(array), array, nodata).astype(dtype)
    profile = base_profile.copy()
    profile.update(count=1, dtype=dtype, nodata=nodata, compress="lzw")
    with rasterio.open(path, "w", **profile) as dataset:
        dataset.write(output, 1)
