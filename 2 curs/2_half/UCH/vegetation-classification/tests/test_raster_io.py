from __future__ import annotations

from pathlib import Path

import numpy as np
from rasterio.transform import from_origin

from src.satellite.bands import SentinelBandPaths
from src.satellite.raster import load_raster_bands


def test_load_raster_bands_aligns_10m_bands_to_20m_reference(
    tmp_path: Path,
    write_raster,
) -> None:
    transform_10m = from_origin(500_000, 40, 10, 10)
    transform_20m = from_origin(500_000, 40, 20, 20)

    paths = SentinelBandPaths(
        blue=write_raster(
            tmp_path / "B02.tif",
            np.full((4, 4), 2, dtype="uint16"),
            transform=transform_10m,
        ),
        green=write_raster(
            tmp_path / "B03.tif",
            np.full((4, 4), 3, dtype="uint16"),
            transform=transform_10m,
        ),
        red=write_raster(
            tmp_path / "B04.tif",
            np.full((4, 4), 4, dtype="uint16"),
            transform=transform_10m,
        ),
        nir=write_raster(
            tmp_path / "B08.tif",
            np.full((4, 4), 8, dtype="uint16"),
            transform=transform_10m,
        ),
        swir1=write_raster(
            tmp_path / "B11.tif",
            np.array([[11, 12], [13, 14]], dtype="uint16"),
            transform=transform_20m,
        ),
        swir2=write_raster(
            tmp_path / "B12.tif",
            np.array([[21, 22], [23, 24]], dtype="uint16"),
            transform=transform_20m,
        ),
        scene_classification=write_raster(
            tmp_path / "SCL.tif",
            np.array([[4, 9], [5, 6]], dtype="uint8"),
            transform=transform_20m,
        ),
    )

    scene = load_raster_bands(paths)

    assert scene.red.shape == (2, 2)
    assert scene.profile["width"] == 2
    assert scene.profile["height"] == 2
    assert scene.profile["transform"] == transform_20m
    np.testing.assert_allclose(scene.blue, 2.0)
    np.testing.assert_allclose(scene.green, 3.0)
    np.testing.assert_allclose(scene.red, 4.0)
    np.testing.assert_allclose(scene.nir, 8.0)
    np.testing.assert_allclose(scene.swir1, [[11.0, 12.0], [13.0, 14.0]])
    np.testing.assert_allclose(scene.swir2, [[21.0, 22.0], [23.0, 24.0]])
    np.testing.assert_array_equal(scene.valid_mask, [[True, False], [True, True]])


def test_ndvi_pair_is_aligned_to_the_red_band_grid(tmp_path: Path, write_raster) -> None:
    paths = SentinelBandPaths(
        red=write_raster(tmp_path / "B04.tif", np.ones((2, 2), dtype="uint16")),
        nir=write_raster(tmp_path / "B08.tif", np.ones((3, 3), dtype="uint16")),
    )

    scene = load_raster_bands(paths)

    assert scene.red.shape == (2, 2)
    assert scene.nir.shape == (2, 2)
