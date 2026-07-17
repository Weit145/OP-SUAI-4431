from __future__ import annotations

from pathlib import Path

import pytest

from src.errors import VegetationError
from src.satellite.bands import (
    SPECTRAL_BAND_CODES,
    discover_default_bands,
    discover_sentinel_bands,
    find_band_candidates,
)


def _touch(product: Path, relative_path: str) -> Path:
    path = product / relative_path
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b"")
    return path


def test_discovery_prefers_processing_resolution_and_img_data(tmp_path: Path) -> None:
    product = tmp_path / "S2_PRODUCT.SAFE"
    img_data = "GRANULE/G1/IMG_DATA"

    _touch(product, f"{img_data}/R10m/T_B02_10m.jp2")
    expected_blue = _touch(product, f"{img_data}/R20m/T_B02_20m.jp2")
    _touch(product, f"{img_data}/R10m/T_B03_10m.jp2")
    expected_green = _touch(product, f"{img_data}/R20m/T_B03_20m.jp2")
    default_red = _touch(product, f"{img_data}/R10m/T_B04_10m.jp2")
    expected_red = _touch(product, f"{img_data}/R20m/T_B04_20m.jp2")
    _touch(product, "loose/T_B04_10m.tif")
    expected_nir = _touch(product, f"{img_data}/R10m/T_B08_10m.jp2")
    _touch(product, f"{img_data}/R10m/T_B11_10m.jp2")
    expected_swir1 = _touch(product, f"{img_data}/R20m/T_B11_20m.jp2")
    expected_swir2 = _touch(product, f"{img_data}/R20m/T_B12_20m.jp2")
    _touch(product, f"{img_data}/R10m/T_SCL_10m.jp2")
    expected_scl = _touch(product, f"{img_data}/R20m/T_SCL_20m.jp2")
    _touch(product, f"{img_data}/R10m/T_B8A_10m.jp2")
    mask_file = _touch(product, f"{img_data}/R20m/MSK_B04_20m.tif")

    paths = discover_sentinel_bands(product, require_multispectral=True)

    assert paths.blue == expected_blue
    assert paths.green == expected_green
    assert paths.red == expected_red
    assert paths.nir == expected_nir
    assert paths.swir1 == expected_swir1
    assert paths.swir2 == expected_swir2
    assert paths.scene_classification == expected_scl
    candidates = find_band_candidates(product, "B04")
    assert candidates[0] == default_red
    assert mask_file not in candidates


def test_discovery_allows_ndvi_pair_but_can_require_full_scene(tmp_path: Path) -> None:
    product = tmp_path / "pair"
    red = _touch(product, "B04.tif")
    nir = _touch(product, "B08.tif")

    paths = discover_sentinel_bands(product)

    assert paths.red == red
    assert paths.nir == nir
    assert not paths.is_multispectral
    with pytest.raises(VegetationError, match="B02"):
        discover_sentinel_bands(product, require_multispectral=True)


def test_default_discovery_accepts_standard_safe_directory_name(tmp_path: Path) -> None:
    product = tmp_path / "S2A_MSIL2A_EXAMPLE.SAFE"
    red = _touch(product, "GRANULE/G1/IMG_DATA/R10m/T_B04_10m.jp2")
    nir = _touch(product, "GRANULE/G1/IMG_DATA/R10m/T_B08_10m.jp2")

    paths = discover_default_bands(tmp_path)

    assert paths.red == red
    assert paths.nir == nir


def test_discovery_does_not_mix_multiple_complete_granules(tmp_path: Path) -> None:
    product = tmp_path / "S2_PRODUCT.SAFE"
    for granule in ("G1", "G2"):
        for band_code in SPECTRAL_BAND_CODES:
            resolution = "10m" if band_code == "B08" else "20m"
            _touch(
                product,
                f"GRANULE/{granule}/IMG_DATA/R{resolution}/T_{band_code}_{resolution}.jp2",
            )

    with pytest.raises(VegetationError, match="несколько гранул"):
        discover_sentinel_bands(product, require_multispectral=True)
