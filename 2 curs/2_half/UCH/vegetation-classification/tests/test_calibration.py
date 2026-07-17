from __future__ import annotations

from pathlib import Path

import numpy as np

from src.satellite.bands import SentinelBandPaths
from src.satellite.calibration import discover_l2a_calibration, parse_l2a_calibration
from src.satellite.raster import load_raster_bands


def _metadata_xml() -> str:
    offsets = "\n".join(
        f'<BOA_ADD_OFFSET band_id="{band_id}">-1000</BOA_ADD_OFFSET>' for band_id in range(13)
    )
    return (
        "<Level-2A_User_Product>"
        "<BOA_QUANTIFICATION_VALUE>10000</BOA_QUANTIFICATION_VALUE>"
        f"<BOA_ADD_OFFSET_VALUES_LIST>{offsets}</BOA_ADD_OFFSET_VALUES_LIST>"
        "</Level-2A_User_Product>"
    )


def test_parse_l2a_calibration_reads_offsets_and_quantification(tmp_path: Path) -> None:
    metadata = tmp_path / "MTD_MSIL2A.xml"
    metadata.write_text(_metadata_xml(), encoding="utf-8")

    calibration = parse_l2a_calibration(metadata)

    assert calibration is not None
    assert calibration.coefficients("B04") == (0.0001, -0.1)
    assert calibration.coefficients("B12") == (0.0001, -0.1)


def test_discovery_uses_processing_baseline_fallback(tmp_path: Path) -> None:
    metadata = tmp_path / "MTD_TL.xml"
    metadata.write_text("<TILE_ID>S2C_TEST_N05.12</TILE_ID>", encoding="utf-8")

    calibration = discover_l2a_calibration(tmp_path)

    assert calibration is not None
    assert "05.12" in calibration.source
    assert calibration.coefficients("B08") == (0.0001, -0.1)


def test_raster_loader_applies_boa_offset_and_preserves_dn_zero_as_nodata(
    tmp_path: Path,
    write_raster,
) -> None:
    metadata = tmp_path / "MTD_MSIL2A.xml"
    metadata.write_text(_metadata_xml(), encoding="utf-8")
    calibration = parse_l2a_calibration(metadata)
    assert calibration is not None
    paths = SentinelBandPaths(
        red=write_raster(
            tmp_path / "B04.tif",
            np.array([[2_000, 0]], dtype="uint16"),
        ),
        nir=write_raster(
            tmp_path / "B08.tif",
            np.array([[6_000, 0]], dtype="uint16"),
        ),
        calibration=calibration,
    )

    scene = load_raster_bands(paths)

    np.testing.assert_allclose(scene.red[0, 0], 0.1, rtol=1e-6)
    np.testing.assert_allclose(scene.nir[0, 0], 0.5, rtol=1e-6)
    np.testing.assert_array_equal(scene.valid_mask, [[True, False]])
