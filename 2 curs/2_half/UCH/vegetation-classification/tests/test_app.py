import io
import zipfile
from pathlib import Path

import numpy as np
from streamlit.testing.v1 import AppTest

from app import process_uploads
from download_data import SPECTRAL_BANDS, create_demo_data


class UploadedBytes(io.BytesIO):
    def __init__(self, payload: bytes, name: str) -> None:
        super().__init__(payload)
        self.name = name


def test_streamlit_app_renders_without_exceptions() -> None:
    app_path = Path(__file__).parents[1] / "app.py"
    app = AppTest.from_file(str(app_path)).run(timeout=30)

    assert not app.exception
    assert len(app.get("file_uploader")) == 2
    assert len(app.button) == 1


def test_app_processes_uploaded_ndvi_pair(tmp_path: Path, write_raster) -> None:
    red_path = write_raster(
        tmp_path / "B04.tif",
        np.array([[2, 1], [4, 3]], dtype="uint16"),
    )
    nir_path = write_raster(
        tmp_path / "B08.tif",
        np.array([[6, 3], [2, 9]], dtype="uint16"),
    )

    result = process_uploads(
        "ndvi",
        red_file=UploadedBytes(red_path.read_bytes(), "B04.tif"),
        nir_file=UploadedBytes(nir_path.read_bytes(), "B08.tif"),
    )

    assert result.mode == "ndvi"
    assert set(result.previews) == {"ndvi", "classification"}
    with zipfile.ZipFile(io.BytesIO(result.report_zip)) as report:
        assert set(report.namelist()) == {
            "ndvi_map.png",
            "classified_map.png",
            "class_statistics.csv",
            "result_summary.txt",
        }


def test_app_processes_multispectral_zip(tmp_path: Path) -> None:
    input_dir = tmp_path / "scene"
    create_demo_data(input_dir, width=30, height=20)
    archive_buffer = io.BytesIO()
    with zipfile.ZipFile(archive_buffer, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for band_code in SPECTRAL_BANDS:
            resolution = "10m" if band_code == "B08" else "20m"
            archive.write(
                input_dir / f"{band_code}.tif",
                arcname=(
                    "S2A_TEST.SAFE/GRANULE/G1/IMG_DATA/"
                    f"R{resolution}/T_TEST_{band_code}_{resolution}.tif"
                ),
            )

    result = process_uploads(
        "multispectral",
        zip_file=UploadedBytes(archive_buffer.getvalue(), "scene.zip"),
    )

    assert result.mode == "multispectral"
    assert set(result.previews) == {"ndvi", "water", "built_up", "classification"}
    assert int(result.statistics["pixel_count"].sum()) == 600
