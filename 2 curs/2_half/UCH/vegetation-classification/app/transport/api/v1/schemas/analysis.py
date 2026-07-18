from __future__ import annotations

import base64

from pydantic import BaseModel, Field

from app.domain.analysis import AnalysisResult


class ClassStatisticResponse(BaseModel):
    class_id: int
    key: str
    name: str
    condition: str
    color: str
    pixel_count: int
    percent: float


class AnalysisResponse(BaseModel):
    mode: str
    valid_pixel_count: int = Field(description="Количество валидных пикселей")
    statistics: list[ClassStatisticResponse]
    previews: dict[str, str]

    @classmethod
    def from_domain(cls, result: AnalysisResult) -> "AnalysisResponse":
        return cls(
            mode=result.mode.value,
            valid_pixel_count=result.valid_pixel_count,
            statistics=[
                ClassStatisticResponse(
                    class_id=item.class_id,
                    key=item.key,
                    name=item.name,
                    condition=item.condition,
                    color=item.color,
                    pixel_count=item.pixel_count,
                    percent=item.percent,
                )
                for item in result.statistics
            ],
            previews={
                preview.key: (
                    "data:image/png;base64,"
                    + base64.b64encode(preview.content).decode("ascii")
                )
                for preview in result.previews
            },
        )
