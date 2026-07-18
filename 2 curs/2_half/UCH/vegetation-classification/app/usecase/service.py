from __future__ import annotations

import logging
import tempfile
from pathlib import Path
from typing import BinaryIO

from app.core.config import settings
from app.domain.analysis import AnalysisResult
from app.domain.errors import VegetationError
from app.infrastructure.satellite import SentinelBandPaths, extract_sentinel_bands
from app.usecase.pipeline import run_pipeline


logger = logging.getLogger(__name__)
RASTER_EXTENSIONS = {".jp2", ".tif", ".tiff"}


class AnalysisService:
    def __init__(self, max_upload_size: int):
        self.max_upload_size = max_upload_size

    def _save_upload(
        self,
        source: BinaryIO,
        filename: str | None,
        destination_stem: Path,
        allowed_extensions: set[str],
    ) -> Path:
        extension = Path(filename or "").suffix.lower()
        if extension not in allowed_extensions:
            expected = ", ".join(sorted(allowed_extensions))
            raise VegetationError(f"Неподдерживаемый формат файла. Допустимы: {expected}.")

        destination = destination_stem.with_suffix(extension)
        source.seek(0)
        written = 0
        try:
            with destination.open("wb") as target:
                while chunk := source.read(1024 * 1024):
                    written += len(chunk)
                    if written > self.max_upload_size:
                        raise VegetationError(
                            f"Файл превышает лимит {self.max_upload_size // 1024 // 1024} МБ."
                        )
                    target.write(chunk)
        except Exception:
            destination.unlink(missing_ok=True)
            raise
        if written == 0:
            destination.unlink(missing_ok=True)
            raise VegetationError("Загружен пустой файл.")
        return destination

    def analyze_ndvi(
        self,
        red_source: BinaryIO,
        red_filename: str | None,
        nir_source: BinaryIO,
        nir_filename: str | None,
    ) -> AnalysisResult:
        with tempfile.TemporaryDirectory(prefix="sentinel-input-") as temporary_dir:
            input_dir = Path(temporary_dir)
            red = self._save_upload(
                red_source, red_filename, input_dir / "B04", RASTER_EXTENSIONS
            )
            nir = self._save_upload(
                nir_source, nir_filename, input_dir / "B08", RASTER_EXTENSIONS
            )
            paths = SentinelBandPaths(red=red, nir=nir)
            result = run_pipeline(paths)
        logger.info("NDVI analysis completed")
        return result

    def analyze_multispectral(
        self,
        archive_source: BinaryIO,
        archive_filename: str | None,
    ) -> AnalysisResult:
        with tempfile.TemporaryDirectory(prefix="sentinel-input-") as temporary_dir:
            input_dir = Path(temporary_dir)
            archive_path = self._save_upload(
                archive_source,
                archive_filename,
                input_dir / "sentinel",
                {".zip"},
            )
            paths = extract_sentinel_bands(archive_path, input_dir / "bands")
            result = run_pipeline(paths)
        logger.info("Multispectral analysis completed")
        return result


analysis_service = AnalysisService(settings.max_upload_size_bytes)
