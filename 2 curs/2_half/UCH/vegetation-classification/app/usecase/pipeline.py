from __future__ import annotations

import numpy as np

from app.domain.analysis import AnalysisMode, AnalysisResult, ClassStatistic, PreviewImage
from app.domain.classification import (
    LAND_COVER_CLASSES,
    VEGETATION_CLASSES,
    calculate_statistics,
    classify_land_cover,
    classify_ndvi,
)
from app.domain.errors import VegetationError
from app.domain.indices import SpectralIndices, calculate_ndvi, calculate_spectral_indices
from app.infrastructure.raster import LoadedRasterBands, load_raster_bands
from app.infrastructure.satellite.bands import SentinelBandPaths
from app.infrastructure.visualization import render_classified_map, render_index_map


def _calculate_all_indices(scene: LoadedRasterBands) -> SpectralIndices:
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


def _run_ndvi(
    scene: LoadedRasterBands,
) -> tuple[tuple[ClassStatistic, ...], tuple[PreviewImage, ...]]:
    ndvi = calculate_ndvi(scene.red, scene.nir, scene.valid_mask)
    classified = classify_ndvi(ndvi, scene.valid_mask)
    statistics = calculate_statistics(classified, VEGETATION_CLASSES)
    previews = (
        PreviewImage(
            key="classification",
            content=render_classified_map(
                classified,
                classes=VEGETATION_CLASSES,
                title="Классификация растительности по NDVI",
            ),
        ),
        PreviewImage(
            key="ndvi",
            content=render_index_map(
                ndvi,
                title="Карта NDVI",
                label="NDVI",
            ),
        ),
    )
    return statistics, previews


def _run_multispectral(
    scene: LoadedRasterBands,
) -> tuple[tuple[ClassStatistic, ...], tuple[PreviewImage, ...]]:
    indices = _calculate_all_indices(scene)
    classified = classify_land_cover(indices, scene.valid_mask)
    statistics = calculate_statistics(classified, LAND_COVER_CLASSES)
    previews = (
        PreviewImage(
            key="classification",
            content=render_classified_map(
                classified,
                classes=LAND_COVER_CLASSES,
                title="Классификация поверхности Sentinel-2",
            ),
        ),
        PreviewImage(
            key="ndvi",
            content=render_index_map(
                indices.ndvi,
                title="Карта NDVI",
                label="NDVI",
            ),
        ),
        PreviewImage(
            key="water",
            content=render_index_map(
                indices.mndwi,
                title="Водный индекс MNDWI",
                label="MNDWI",
                cmap="BrBG_r",
            ),
        ),
        PreviewImage(
            key="built_up",
            content=render_index_map(
                indices.ndbi,
                title="Индекс застройки NDBI",
                label="NDBI",
                cmap="PuOr_r",
            ),
        ),
    )
    return statistics, previews


def run_pipeline(paths: SentinelBandPaths) -> AnalysisResult:
    scene = load_raster_bands(paths)
    if not np.any(scene.valid_mask):
        raise VegetationError("После маскирования в снимке не осталось валидных пикселей.")

    if scene.is_multispectral:
        mode = AnalysisMode.MULTISPECTRAL
        statistics, previews = _run_multispectral(scene)
    else:
        mode = AnalysisMode.NDVI
        statistics, previews = _run_ndvi(scene)

    return AnalysisResult(
        mode=mode,
        statistics=statistics,
        previews=previews,
    )
