from __future__ import annotations

from collections.abc import Callable
from dataclasses import dataclass
from pathlib import Path

import pandas as pd

from src.processing.classification import (
    LAND_COVER_CLASSES,
    calculate_land_cover_statistics,
    calculate_statistics,
    classify_land_cover,
    classify_ndvi,
)
from src.processing.indices import SpectralIndices, calculate_ndvi, calculate_spectral_indices
from src.satellite.bands import SentinelBandPaths
from src.satellite.raster import LoadedRasterBands, load_raster_bands, save_geotiff
from src.visualization import save_classified_map, save_index_map, save_ndvi_map


ProgressCallback = Callable[[str], None]


@dataclass(frozen=True)
class PipelineResult:
    """Данные, которые нужны интерфейсу после обработки."""

    mode: str
    statistics: pd.DataFrame
    summary_text: str
    preview_paths: dict[str, Path]


def _notify(callback: ProgressCallback | None, message: str) -> None:
    if callback is not None:
        callback(message)


def _calculate_all_indices(scene: LoadedRasterBands) -> SpectralIndices:
    """Передать шесть уже совмещённых каналов в модуль формул."""

    assert scene.blue is not None
    assert scene.green is not None
    assert scene.swir1 is not None
    assert scene.swir2 is not None
    return calculate_spectral_indices(
        blue=scene.blue,
        green=scene.green,
        red=scene.red,
        nir=scene.nir,
        swir1=scene.swir1,
        swir2=scene.swir2,
        valid_mask=scene.valid_mask,
    )


def _summary(
    paths: SentinelBandPaths,
    statistics: pd.DataFrame,
    mode: str,
) -> str:
    lines = ["Отчёт по классификации спутникового снимка", "", "Входные каналы:"]
    lines.extend(f"- {name}: {path}" for name, path in paths.spectral_paths().items())

    if paths.calibration is not None:
        lines.append(f"- Калибровка: {paths.calibration.source}")

    lines.extend(["", "Метод:"])
    if mode == "multispectral":
        lines.append("- NDVI, NDWI, MNDWI, NDBI, BSI и Urban Index")
        lines.append("- Пороговая классификация воды, почвы, застройки и растительности")
    else:
        lines.append("- NDVI = (B08 - B04) / (B08 + B04)")
    if paths.scene_classification is not None:
        lines.append("- Облака, тени и снег исключены по SCL")

    lines.extend(["", "Распределение классов:"])
    for row in statistics.to_dict("records"):
        lines.append(f"- {row['class_name']}: {row['pixel_count']} пикс., {row['percent']}%")

    if mode == "multispectral":
        lines.extend(["", "Классификация эвристическая; пороги зависят от региона и сезона."])
    return "\n".join(lines) + "\n"


def _save_report(out_dir: Path, statistics: pd.DataFrame, summary_text: str) -> None:
    statistics.to_csv(out_dir / "class_statistics.csv", index=False, encoding="utf-8-sig")
    (out_dir / "result_summary.txt").write_text(summary_text, encoding="utf-8")


def _remove_old_mode_files(mode: str, out_dir: Path, processed_dir: Path) -> None:
    """Не оставлять рядом файлы от ранее запущенного другого режима."""

    names = (
        ("classified_vegetation.tif",)
        if mode == "multispectral"
        else (
            "water_index_map.png",
            "built_up_index_map.png",
            "ndwi.tif",
            "mndwi.tif",
            "ndbi.tif",
            "bsi.tif",
            "urban_index.tif",
            "classified_land_cover.tif",
        )
    )
    for name in names:
        folder = out_dir if name.endswith(".png") else processed_dir
        (folder / name).unlink(missing_ok=True)


def _run_ndvi(
    scene: LoadedRasterBands,
    out_dir: Path,
    processed_dir: Path,
) -> tuple[pd.DataFrame, dict[str, Path]]:
    ndvi = calculate_ndvi(scene.red, scene.nir, scene.valid_mask)
    classified = classify_ndvi(ndvi, scene.valid_mask)
    statistics = calculate_statistics(classified)

    previews = {
        "ndvi": out_dir / "ndvi_map.png",
        "classification": out_dir / "classified_map.png",
    }
    save_ndvi_map(ndvi, previews["ndvi"])
    save_classified_map(classified, previews["classification"])
    save_geotiff(
        processed_dir / "ndvi.tif",
        ndvi,
        scene.profile,
        dtype="float32",
        nodata=-9999.0,
    )
    save_geotiff(
        processed_dir / "classified_vegetation.tif",
        classified,
        scene.profile,
        dtype="uint8",
        nodata=0,
    )
    return statistics, previews


def _run_multispectral(
    scene: LoadedRasterBands,
    out_dir: Path,
    processed_dir: Path,
) -> tuple[pd.DataFrame, dict[str, Path]]:
    indices = _calculate_all_indices(scene)
    classified = classify_land_cover(indices, scene.valid_mask)
    statistics = calculate_land_cover_statistics(classified)

    previews = {
        "ndvi": out_dir / "ndvi_map.png",
        "water": out_dir / "water_index_map.png",
        "built_up": out_dir / "built_up_index_map.png",
        "classification": out_dir / "classified_map.png",
    }
    save_ndvi_map(indices.ndvi, previews["ndvi"])
    save_index_map(
        indices.mndwi,
        previews["water"],
        title="Водный индекс MNDWI",
        label="MNDWI",
        cmap="BrBG_r",
    )
    save_index_map(
        indices.ndbi,
        previews["built_up"],
        title="Индекс застройки NDBI",
        label="NDBI",
        cmap="PuOr_r",
    )
    save_classified_map(
        classified,
        previews["classification"],
        classes=LAND_COVER_CLASSES,
        title="Классификация поверхности Sentinel-2",
    )

    for name, array in indices.as_dict().items():
        save_geotiff(
            processed_dir / f"{name}.tif",
            array,
            scene.profile,
            dtype="float32",
            nodata=-9999.0,
        )
    save_geotiff(
        processed_dir / "classified_land_cover.tif",
        classified,
        scene.profile,
        dtype="uint8",
        nodata=0,
    )
    return statistics, previews


def run_pipeline(
    paths: SentinelBandPaths,
    out_dir: Path,
    processed_dir: Path,
    *,
    progress: ProgressCallback | None = None,
) -> PipelineResult:
    """Выполнить все этапы обработки для двух или шести каналов."""

    out_dir.mkdir(parents=True, exist_ok=True)
    processed_dir.mkdir(parents=True, exist_ok=True)

    _notify(progress, "Чтение и совмещение каналов...")
    scene = load_raster_bands(paths)

    if scene.is_multispectral:
        mode = "multispectral"
        _notify(progress, "Расчёт индексов и классов поверхности...")
        statistics, previews = _run_multispectral(scene, out_dir, processed_dir)
    else:
        mode = "ndvi"
        _notify(progress, "Расчёт NDVI и классов растительности...")
        statistics, previews = _run_ndvi(scene, out_dir, processed_dir)

    summary_text = _summary(paths, statistics, mode)
    _save_report(out_dir, statistics, summary_text)
    _remove_old_mode_files(mode, out_dir, processed_dir)
    _notify(progress, "Результаты сохранены.")
    return PipelineResult(mode, statistics, summary_text, previews)
