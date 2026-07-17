from __future__ import annotations

from pathlib import Path

import numpy as np
from rasterio.transform import from_origin

from src.pipeline import run_pipeline
from src.satellite.bands import SentinelBandPaths


def _assert_artifacts_exist(paths: tuple[Path, ...]) -> None:
    assert paths
    for path in paths:
        assert path.is_file(), f"Pipeline did not create {path}"
        assert path.stat().st_size > 0


def test_pipeline_runs_legacy_ndvi_mode(tmp_path: Path, write_raster) -> None:
    transform = from_origin(500_000, 20, 10, 10)
    paths = SentinelBandPaths(
        red=write_raster(
            tmp_path / "input" / "B04.tif",
            np.array([[3, 2], [1, 4]], dtype="uint16"),
            transform=transform,
        ),
        nir=write_raster(
            tmp_path / "input" / "B08.tif",
            np.array([[7, 2], [9, 1]], dtype="uint16"),
            transform=transform,
        ),
    )
    progress: list[str] = []
    stale_paths = (
        tmp_path / "results" / "water_index_map.png",
        tmp_path / "processed" / "ndbi.tif",
        tmp_path / "processed" / "classified_land_cover.tif",
    )
    for stale_path in stale_paths:
        stale_path.parent.mkdir(parents=True, exist_ok=True)
        stale_path.write_bytes(b"stale")

    result = run_pipeline(
        paths,
        tmp_path / "results",
        tmp_path / "processed",
        progress=progress.append,
    )

    assert result.mode == "ndvi"
    assert set(result.preview_paths) == {"ndvi", "classification"}
    assert int(result.statistics["pixel_count"].sum()) == 4
    assert (tmp_path / "results" / "result_summary.txt").read_text(
        encoding="utf-8"
    ) == result.summary_text
    assert progress and len(progress) == 3
    assert not any(path.exists() for path in stale_paths)
    _assert_artifacts_exist(
        (
            *result.preview_paths.values(),
            tmp_path / "results" / "class_statistics.csv",
            tmp_path / "processed" / "ndvi.tif",
            tmp_path / "processed" / "classified_vegetation.tif",
        )
    )


def test_pipeline_runs_multispectral_mode(tmp_path: Path, write_raster) -> None:
    transform = from_origin(500_000, 40, 20, 20)
    input_dir = tmp_path / "input"
    bands = {
        # Columns represent water, built-up, dense vegetation and bare soil.
        "B02": np.array([[0.10, 0.10], [0.05, 0.10]], dtype="float32"),
        "B03": np.array([[0.60, 0.10], [0.10, 0.20]], dtype="float32"),
        "B04": np.array([[0.20, 0.20], [0.10, 0.30]], dtype="float32"),
        "B08": np.array([[0.10, 0.20], [0.80, 0.30]], dtype="float32"),
        "B11": np.array([[0.10, 0.50], [0.20, 0.50]], dtype="float32"),
        "B12": np.array([[0.05, 0.60], [0.20, 0.20]], dtype="float32"),
    }
    raster_paths = {
        code: write_raster(input_dir / f"{code}.tif", values, transform=transform)
        for code, values in bands.items()
    }
    paths = SentinelBandPaths(
        blue=raster_paths["B02"],
        green=raster_paths["B03"],
        red=raster_paths["B04"],
        nir=raster_paths["B08"],
        swir1=raster_paths["B11"],
        swir2=raster_paths["B12"],
    )

    result = run_pipeline(paths, tmp_path / "results", tmp_path / "processed")

    assert result.mode == "multispectral"
    assert set(result.preview_paths) == {"ndvi", "water", "built_up", "classification"}
    counts = result.statistics.set_index("class_key")["pixel_count"].to_dict()
    assert counts["water"] == 1
    assert counts["built_up"] == 1
    assert counts["dense_vegetation"] == 1
    assert counts["bare_soil"] == 1
    expected_geotiffs = {
        "ndvi.tif",
        "ndwi.tif",
        "mndwi.tif",
        "ndbi.tif",
        "bsi.tif",
        "urban_index.tif",
        "classified_land_cover.tif",
    }
    assert expected_geotiffs <= {
        path.name for path in (tmp_path / "processed").iterdir() if path.is_file()
    }
    _assert_artifacts_exist(tuple(result.preview_paths.values()))
