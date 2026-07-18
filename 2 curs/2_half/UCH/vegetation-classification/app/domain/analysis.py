from __future__ import annotations

from dataclasses import dataclass
from enum import StrEnum


class AnalysisMode(StrEnum):
    NDVI = "ndvi"
    MULTISPECTRAL = "multispectral"


@dataclass(frozen=True, slots=True)
class ClassStatistic:
    class_id: int
    key: str
    name: str
    condition: str
    color: str
    pixel_count: int
    percent: float


@dataclass(frozen=True, slots=True)
class PreviewImage:
    key: str
    content: bytes


@dataclass(frozen=True, slots=True)
class AnalysisResult:
    mode: AnalysisMode
    statistics: tuple[ClassStatistic, ...]
    previews: tuple[PreviewImage, ...]

    @property
    def valid_pixel_count(self) -> int:
        return sum(item.pixel_count for item in self.statistics)
