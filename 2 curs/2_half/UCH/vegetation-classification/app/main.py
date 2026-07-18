from __future__ import annotations

import logging
from contextlib import asynccontextmanager

from fastapi import FastAPI, Request, status
from fastapi.responses import JSONResponse

from app.core.config import settings
from app.core.logging import setup_logging
from app.domain.errors import VegetationError
from app.transport.api.v1.handler import router as analyses_router


setup_logging()
logger = logging.getLogger(__name__)


@asynccontextmanager
async def lifespan(_: FastAPI):
    logger.info("Application started")
    yield
    logger.info("Application stopped")


app = FastAPI(
    lifespan=lifespan,
    title=settings.app_name,
    version=settings.app_version,
    description="API для расчёта NDVI и классификации снимков Sentinel-2.",
)
app.include_router(analyses_router)


@app.exception_handler(VegetationError)
async def vegetation_error_handler(_: Request, error: VegetationError) -> JSONResponse:
    return JSONResponse(
        status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
        content={"detail": str(error)},
    )


@app.get("/_info", tags=["System"], summary="Проверить состояние сервиса")
async def info() -> dict[str, str]:
    return {
        "status": "ok",
        "service": settings.app_name,
        "version": settings.app_version,
    }
