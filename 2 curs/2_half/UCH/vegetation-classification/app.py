from __future__ import annotations

import io
import shutil
import tempfile
import zipfile
from dataclasses import dataclass
from pathlib import Path

import pandas as pd
import streamlit as st

from src.errors import VegetationError
from src.pipeline import PipelineResult, run_pipeline
from src.satellite.archive import extract_sentinel_bands
from src.satellite.bands import SentinelBandPaths
from src.satellite.calibration import modern_l2a_calibration


SESSION_RESULT_KEY = "analysis_result"
SESSION_MODE_KEY = "analysis_mode"


@dataclass(frozen=True)
class WebResult:
    mode: str
    statistics: pd.DataFrame
    summary_text: str
    previews: dict[str, bytes]
    report_zip: bytes


def _save_uploaded_raster(uploaded_file, destination: Path) -> Path:
    suffix = Path(uploaded_file.name).suffix.lower() or ".tif"
    path = destination.with_suffix(suffix)
    uploaded_file.seek(0)
    with path.open("wb") as target:
        shutil.copyfileobj(uploaded_file, target, length=1024 * 1024)
    return path


def _read_previews(result: PipelineResult) -> dict[str, bytes]:
    return {name: path.read_bytes() for name, path in result.preview_paths.items()}


def _build_report_zip(
    result: PipelineResult,
    previews: dict[str, bytes],
) -> bytes:
    buffer = io.BytesIO()
    preview_names = {
        "ndvi": "ndvi_map.png",
        "water": "water_index_map.png",
        "built_up": "built_up_index_map.png",
        "classification": "classified_map.png",
    }
    with zipfile.ZipFile(buffer, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for key, image_bytes in previews.items():
            archive.writestr(preview_names[key], image_bytes)
        archive.writestr(
            "class_statistics.csv",
            result.statistics.to_csv(index=False).encode("utf-8-sig"),
        )
        archive.writestr("result_summary.txt", result.summary_text.encode("utf-8"))
    return buffer.getvalue()


def process_uploads(
    mode: str,
    *,
    red_file=None,
    nir_file=None,
    zip_file=None,
    apply_boa_offset: bool = False,
    progress=None,
) -> WebResult:
    """Process uploaded files entirely inside an isolated temporary directory."""

    with tempfile.TemporaryDirectory(prefix="vegetation_") as temporary_dir:
        work_dir = Path(temporary_dir)
        input_dir = work_dir / "input"
        input_dir.mkdir()

        if mode == "ndvi":
            if red_file is None or nir_file is None:
                raise VegetationError("Загрузите оба файла: B04 и B08.")
            paths = SentinelBandPaths(
                red=_save_uploaded_raster(red_file, input_dir / "B04"),
                nir=_save_uploaded_raster(nir_file, input_dir / "B08"),
                calibration=(
                    modern_l2a_calibration("настройка Streamlit для PB >= 04.00")
                    if apply_boa_offset
                    else None
                ),
            )
        elif mode == "multispectral":
            if zip_file is None:
                raise VegetationError("Загрузите ZIP с продуктом Sentinel-2.")
            zip_file.seek(0)
            paths = extract_sentinel_bands(zip_file, input_dir)
        else:
            raise ValueError(f"Unknown processing mode: {mode}")

        result = run_pipeline(
            paths,
            out_dir=work_dir / "results",
            processed_dir=work_dir / "processed",
            progress=progress,
        )
        previews = _read_previews(result)
        return WebResult(
            mode=result.mode,
            statistics=result.statistics.copy(),
            summary_text=result.summary_text,
            previews=previews,
            report_zip=_build_report_zip(result, previews),
        )


def _statistics_for_display(statistics: pd.DataFrame) -> pd.DataFrame:
    columns = ["class_name"]
    if "ndvi_range" in statistics:
        columns.append("ndvi_range")
    if "condition" in statistics:
        columns.append("condition")
    columns.extend(["pixel_count", "percent"])
    return statistics[columns].rename(
        columns={
            "class_name": "Класс",
            "ndvi_range": "Диапазон NDVI",
            "condition": "Правило",
            "pixel_count": "Пиксели",
            "percent": "Процент",
        }
    )


def _render_result(result: WebResult) -> None:
    st.subheader("Результат")
    total_pixels = int(result.statistics["pixel_count"].sum())
    mode_label = "Полная классификация" if result.mode == "multispectral" else "NDVI"
    metric_mode, metric_pixels = st.columns(2)
    metric_mode.metric("Режим", mode_label)
    metric_pixels.metric("Валидных пикселей", f"{total_pixels:,}".replace(",", " "))

    labels = {
        "classification": "Классификация",
        "ndvi": "NDVI",
        "water": "Вода (MNDWI)",
        "built_up": "Застройка (NDBI)",
    }
    ordered_keys = [
        key for key in ("classification", "ndvi", "water", "built_up") if key in result.previews
    ]
    tabs = st.tabs([labels[key] for key in ordered_keys])
    for tab, key in zip(tabs, ordered_keys, strict=True):
        with tab:
            st.image(result.previews[key], use_container_width=True)

    st.subheader("Статистика классов")
    st.dataframe(
        _statistics_for_display(result.statistics),
        hide_index=True,
        use_container_width=True,
    )
    with st.expander("Текстовый отчёт"):
        st.text(result.summary_text)

    st.download_button(
        "Скачать отчёт ZIP",
        data=result.report_zip,
        file_name="sentinel_analysis_report.zip",
        mime="application/zip",
        use_container_width=True,
    )


def main() -> None:
    st.set_page_config(page_title="Классификация Sentinel-2", page_icon="🛰️", layout="wide")
    st.title("Классификация спутниковых снимков Sentinel-2")
    st.write(
        "Рассчитайте NDVI либо выполните полную классификацию воды, почвы, "
        "застройки и растительности."
    )

    mode_label = st.radio(
        "Выберите режим",
        (
            "B04 + B08 — NDVI и растительность",
            "ZIP Sentinel-2 — полная классификация",
        ),
    )
    mode = "ndvi" if mode_label.startswith("B04") else "multispectral"
    if st.session_state.get(SESSION_MODE_KEY) not in (None, mode):
        st.session_state.pop(SESSION_RESULT_KEY, None)
    st.session_state[SESSION_MODE_KEY] = mode

    with st.form("analysis_form"):
        red_file = nir_file = zip_file = None
        apply_boa_offset = False
        if mode == "ndvi":
            st.info(
                "По двум каналам рассчитывается NDVI. Надёжно разделить воду, "
                "почву и здания в этом режиме нельзя."
            )
            red_column, nir_column = st.columns(2)
            with red_column:
                red_file = st.file_uploader("Красный канал B04", type=["jp2", "tif", "tiff"])
            with nir_column:
                nir_file = st.file_uploader(
                    "Ближний инфракрасный канал B08",
                    type=["jp2", "tif", "tiff"],
                )
            with st.expander("Дополнительная настройка"):
                apply_boa_offset = st.checkbox(
                    "Применить BOA offset",
                    value=True,
                    help=(
                        "Оставьте включённым для исходных JP2 Sentinel-2 L2A. "
                        "Отключите для уже откалиброванных GeoTIFF."
                    ),
                )
        else:
            st.info(
                "ZIP должен содержать одну сцену и каналы "
                "B02, B03, B04, B08, B11 и B12. Маска SCL используется автоматически."
            )
            zip_file = st.file_uploader("ZIP продукта Sentinel-2", type=["zip"])

        submitted = st.form_submit_button("Обработать", type="primary", use_container_width=True)

    if submitted:
        st.session_state.pop(SESSION_RESULT_KEY, None)
        status = st.status("Подготовка данных...", expanded=True)
        try:
            result = process_uploads(
                mode,
                red_file=red_file,
                nir_file=nir_file,
                zip_file=zip_file,
                apply_boa_offset=apply_boa_offset,
                progress=status.write,
            )
            st.session_state[SESSION_RESULT_KEY] = result
            status.update(label="Обработка завершена", state="complete", expanded=False)
        except VegetationError as error:
            status.update(label="Ошибка обработки", state="error", expanded=True)
            st.error(str(error))
        except Exception as error:
            status.update(label="Непредвиденная ошибка", state="error", expanded=True)
            st.error("Не удалось обработать данные из-за технической ошибки.")
            st.caption(str(error))

    if SESSION_RESULT_KEY in st.session_state:
        _render_result(st.session_state[SESSION_RESULT_KEY])


if __name__ == "__main__":
    main()
