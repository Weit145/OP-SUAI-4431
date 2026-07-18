from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


REQUIRED_BAND_CODES = ("B02", "B03", "B04", "B08", "B11", "B12")
SUPPORTED_RASTERS = {".jp2", ".tif", ".tiff"}


@dataclass(frozen=True, slots=True)
class SentinelBandPaths:
    red: Path
    nir: Path
    blue: Path | None = None
    green: Path | None = None
    swir1: Path | None = None
    swir2: Path | None = None

    @property
    def is_multispectral(self) -> bool:
        return all((self.blue, self.green, self.swir1, self.swir2))

    def as_dict(self) -> dict[str, Path]:
        bands = {
            "blue": self.blue,
            "green": self.green,
            "red": self.red,
            "nir": self.nir,
            "swir1": self.swir1,
            "swir2": self.swir2,
        }
        return {name: path for name, path in bands.items() if path is not None}
