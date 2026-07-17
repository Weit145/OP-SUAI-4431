from __future__ import annotations

import math
import re
from dataclasses import dataclass
from pathlib import Path


BAND_ID_TO_CODE = {
    0: "B01",
    1: "B02",
    2: "B03",
    3: "B04",
    4: "B05",
    5: "B06",
    6: "B07",
    7: "B08",
    8: "B8A",
    9: "B09",
    10: "B10",
    11: "B11",
    12: "B12",
}
PROCESSING_BANDS = ("B02", "B03", "B04", "B08", "B11", "B12")


@dataclass(frozen=True)
class RadiometricCalibration:
    """Коэффициенты перевода DN Sentinel-2 в отражательную способность."""

    quantification_value: float
    offsets: dict[str, float]
    source: str

    def coefficients(self, band_code: str) -> tuple[float, float]:
        scale = 1.0 / self.quantification_value
        return scale, self.offsets[band_code] * scale


def modern_l2a_calibration(source: str) -> RadiometricCalibration:
    """Стандартные коэффициенты Sentinel-2 L2A для PB 04.00 и новее."""

    return RadiometricCalibration(
        quantification_value=10_000.0,
        offsets={code: -1_000.0 for code in PROCESSING_BANDS},
        source=source,
    )


def _read_metadata(path: Path) -> str | None:
    try:
        # Метаданные Sentinel-2 невелики; лимит защищает от случайного чтения
        # огромного файла вместо XML.
        if path.stat().st_size > 16 * 1024**2:
            return None
        return path.read_text(encoding="utf-8", errors="ignore")
    except OSError:
        return None


def parse_l2a_calibration(metadata_path: Path) -> RadiometricCalibration | None:
    """Прочитать BOA_QUANTIFICATION_VALUE и BOA_ADD_OFFSET из XML."""

    text = _read_metadata(metadata_path)
    if text is None:
        return None

    quantification_match = re.search(
        r"<(?:[\w.-]+:)?BOA_QUANTIFICATION_VALUE\b[^>]*>\s*([^<\s]+)",
        text,
        flags=re.IGNORECASE,
    )
    if quantification_match is None:
        return None

    try:
        quantification = float(quantification_match.group(1))
    except ValueError:
        return None
    if not math.isfinite(quantification) or quantification <= 0:
        return None

    offsets: dict[str, float] = {}
    pattern = re.compile(
        r"<(?:[\w.-]+:)?BOA_ADD_OFFSET\b([^>]*)>\s*([^<\s]+)",
        flags=re.IGNORECASE,
    )
    for match in pattern.finditer(text):
        attributes, value_text = match.groups()
        band_match = re.search(r"band_?[Ii]d\s*=\s*['\"](\d+)['\"]", attributes)
        if band_match is None:
            continue
        code = BAND_ID_TO_CODE.get(int(band_match.group(1)))
        try:
            value = float(value_text)
        except ValueError:
            continue
        if code is not None and math.isfinite(value):
            offsets[code] = value

    if not all(code in offsets for code in PROCESSING_BANDS):
        return None
    return RadiometricCalibration(
        quantification_value=quantification,
        offsets={code: offsets[code] for code in PROCESSING_BANDS},
        source=metadata_path.name,
    )


def _processing_baseline(metadata_path: Path) -> tuple[int, int] | None:
    text = _read_metadata(metadata_path)
    if text is None:
        return None

    for pattern in (
        r"PROCESSING_BASELINE[^0-9]{0,20}(\d{2})\.(\d{2})",
        r"(?:^|_)N(\d{2})[._]?(\d{2})(?:_|<|\s|$)",
    ):
        match = re.search(pattern, text, flags=re.IGNORECASE)
        if match:
            return int(match.group(1)), int(match.group(2))
    return None


def discover_l2a_calibration(product_dir: Path) -> RadiometricCalibration | None:
    """Найти калибровку в XML распакованного продукта."""

    metadata = sorted(
        product_dir.rglob("*.xml"),
        key=lambda path: (path.name.casefold() != "mtd_msil2a.xml", path.as_posix()),
    )
    for path in metadata:
        calibration = parse_l2a_calibration(path)
        if calibration is not None:
            return calibration

    for path in metadata:
        baseline = _processing_baseline(path)
        if baseline is not None and baseline >= (4, 0):
            return modern_l2a_calibration(
                f"{path.name}, processing baseline {baseline[0]:02d}.{baseline[1]:02d}"
            )
    return None
