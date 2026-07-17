from __future__ import annotations

from collections.abc import Callable
from pathlib import Path

import numpy as np
import pytest
import rasterio
from affine import Affine
from rasterio.transform import from_origin


RasterWriter = Callable[..., Path]


@pytest.fixture
def write_raster() -> RasterWriter:
    """Return a small GeoTIFF writer for synthetic, network-free test scenes."""

    def _write_raster(
        path: Path,
        values: np.ndarray | list[list[float]],
        *,
        transform: Affine | None = None,
        crs: str = "EPSG:32636",
        nodata: float | int | None = None,
    ) -> Path:
        array = np.asarray(values)
        if array.ndim != 2:
            raise ValueError("Synthetic raster values must be two-dimensional")

        path.parent.mkdir(parents=True, exist_ok=True)
        raster_transform = transform or from_origin(
            500_000,
            array.shape[0] * 10,
            10,
            10,
        )
        with rasterio.open(
            path,
            "w",
            driver="GTiff",
            width=array.shape[1],
            height=array.shape[0],
            count=1,
            dtype=array.dtype,
            crs=crs,
            transform=raster_transform,
            nodata=nodata,
        ) as dataset:
            dataset.write(array, 1)
        return path

    return _write_raster
