from __future__ import annotations

from typing import Annotated

from fastapi import APIRouter, File, UploadFile, status
from starlette.concurrency import run_in_threadpool

from app.transport.api.v1.schemas.analysis import AnalysisResponse
from app.usecase.service import analysis_service


router = APIRouter(prefix="/api/v1/analyses", tags=["Analyses"])


@router.post(
    "/ndvi",
    response_model=AnalysisResponse,
    status_code=status.HTTP_201_CREATED,
    summary="Рассчитать NDVI по каналам B04 и B08",
)
async def analyze_ndvi(
    red: Annotated[
        UploadFile,
        File(description="Красный канал Sentinel-2 B04 в JP2, TIF или TIFF"),
    ],
    nir: Annotated[
        UploadFile,
        File(description="Ближний инфракрасный канал Sentinel-2 B08 в JP2, TIF или TIFF"),
    ],
) -> AnalysisResponse:
    result = await run_in_threadpool(
        analysis_service.analyze_ndvi,
        red.file,
        red.filename,
        nir.file,
        nir.filename,
    )
    return AnalysisResponse.from_domain(result)


@router.post(
    "/multispectral",
    response_model=AnalysisResponse,
    status_code=status.HTTP_201_CREATED,
    summary="Классифицировать мультиспектральный продукт Sentinel-2",
)
async def analyze_multispectral(
    archive: Annotated[
        UploadFile,
        File(description="ZIP одной сцены с каналами B02, B03, B04, B08, B11 и B12"),
    ],
) -> AnalysisResponse:
    result = await run_in_threadpool(
        analysis_service.analyze_multispectral,
        archive.file,
        archive.filename,
    )
    return AnalysisResponse.from_domain(result)
