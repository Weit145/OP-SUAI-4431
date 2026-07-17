from __future__ import annotations

import zipfile
from pathlib import Path

import pytest

from src.errors import VegetationError
from src.satellite.archive import extract_sentinel_bands
from src.satellite.bands import SPECTRAL_BAND_CODES
from src.satellite.calibration import PROCESSING_BANDS


def _write_flat_zip(path: Path, band_codes: tuple[str, ...]) -> Path:
    with zipfile.ZipFile(path, "w") as archive:
        for band_code in band_codes:
            archive.writestr(f"{band_code}.tif", f"payload:{band_code}".encode())
    return path


def test_extract_sentinel_bands_rejects_path_traversal(tmp_path: Path) -> None:
    source = tmp_path / "unsafe.zip"
    with zipfile.ZipFile(source, "w") as archive:
        archive.writestr("../escaped.txt", b"must not escape")

    with pytest.raises(VegetationError):
        extract_sentinel_bands(source, tmp_path / "destination")

    assert not (tmp_path / "escaped.txt").exists()


def test_extract_sentinel_bands_reports_missing_bands(tmp_path: Path) -> None:
    source = _write_flat_zip(tmp_path / "incomplete.zip", ("B04", "B08"))
    with zipfile.ZipFile(source, "a") as archive:
        archive.writestr("MSK_B02.tif", b"this is a mask, not band B02")

    with pytest.raises(VegetationError, match="B02"):
        extract_sentinel_bands(source, tmp_path / "extracted")


def test_extract_sentinel_bands_accepts_successful_flat_zip(tmp_path: Path) -> None:
    source = _write_flat_zip(tmp_path / "scene.zip", SPECTRAL_BAND_CODES + ("SCL",))

    paths = extract_sentinel_bands(source, tmp_path / "extracted")

    assert paths.is_multispectral
    assert paths.scene_classification == tmp_path / "extracted" / "SCL.tif"
    for band_code, extracted_path in zip(
        SPECTRAL_BAND_CODES,
        (
            paths.blue,
            paths.green,
            paths.red,
            paths.nir,
            paths.swir1,
            paths.swir2,
        ),
        strict=True,
    ):
        assert extracted_path is not None
        assert extracted_path.name == f"{band_code}.tif"
        assert extracted_path.read_bytes() == f"payload:{band_code}".encode()


def test_extract_sentinel_bands_keeps_radiometric_metadata(tmp_path: Path) -> None:
    source = _write_flat_zip(tmp_path / "scene.zip", SPECTRAL_BAND_CODES)
    offsets = "".join(
        f'<BOA_ADD_OFFSET band_id="{band_id}">-1000</BOA_ADD_OFFSET>'
        for band_id in (1, 2, 3, 7, 11, 12)
    )
    metadata = (
        f"<Product><BOA_QUANTIFICATION_VALUE>10000</BOA_QUANTIFICATION_VALUE>{offsets}</Product>"
    )
    with zipfile.ZipFile(source, "a") as archive:
        archive.writestr("SAFE/MTD_MSIL2A.xml", metadata)

    paths = extract_sentinel_bands(source, tmp_path / "extracted")

    assert paths.calibration is not None
    assert set(paths.calibration.offsets) == set(PROCESSING_BANDS)


def test_extract_sentinel_bands_rejects_invalid_zip(tmp_path: Path) -> None:
    source = tmp_path / "invalid.zip"
    source.write_bytes(b"not a zip")

    with pytest.raises(VegetationError, match="корректным ZIP"):
        extract_sentinel_bands(source, tmp_path / "extracted")


def test_extract_sentinel_bands_rejects_multiple_granules(tmp_path: Path) -> None:
    source = tmp_path / "multiple.zip"
    with zipfile.ZipFile(source, "w") as archive:
        for granule in ("G1", "G2"):
            for band_code in SPECTRAL_BAND_CODES:
                resolution = "10m" if band_code == "B08" else "20m"
                archive.writestr(
                    f"SAFE/GRANULE/{granule}/IMG_DATA/R{resolution}/T_{band_code}_{resolution}.tif",
                    band_code,
                )

    with pytest.raises(VegetationError, match="несколько гранул"):
        extract_sentinel_bands(source, tmp_path / "extracted")
