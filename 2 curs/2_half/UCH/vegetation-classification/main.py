from __future__ import annotations

import argparse
import sys
from pathlib import Path

from src.errors import VegetationError
from src.pipeline import run_pipeline
from src.satellite import SentinelBandPaths, discover_default_bands, discover_sentinel_bands
from src.satellite.calibration import modern_l2a_calibration


PROJECT_DIR = Path(__file__).resolve().parent


def configure_terminal_encoding() -> None:
    """Make Russian status messages readable in Windows terminals."""

    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    if hasattr(sys.stderr, "reconfigure"):
        sys.stderr.reconfigure(encoding="utf-8")


def project_path(path_text: str) -> Path:
    path = Path(path_text)
    return path if path.is_absolute() else PROJECT_DIR / path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=("Классификация Sentinel-2 по NDVI и дополнительным спектральным индексам.")
    )
    parser.add_argument(
        "--product",
        help="Название или путь к распакованному продукту Sentinel-2.",
    )
    parser.add_argument("--blue", help="Путь к B02 (синий канал).")
    parser.add_argument("--green", help="Путь к B03 (зелёный канал).")
    parser.add_argument("--red", help="Путь к B04 (красный канал).")
    parser.add_argument("--nir", help="Путь к B08 (ближний инфракрасный канал).")
    parser.add_argument("--swir1", help="Путь к B11 (коротковолновый ИК-канал).")
    parser.add_argument("--swir2", help="Путь к B12 (коротковолновый ИК-канал).")
    parser.add_argument(
        "--apply-boa-offset",
        action="store_true",
        help="Применить к явным DN-файлам калибровку Sentinel-2 L2A PB >= 04.00.",
    )
    parser.add_argument(
        "--out",
        default="results",
        help="Папка для PNG, CSV и отчёта. По умолчанию: results",
    )
    parser.add_argument(
        "--processed",
        default="data/processed",
        help="Папка для расчётных GeoTIFF. По умолчанию: data/processed",
    )
    return parser.parse_args()


def resolve_product_path(product_text: str) -> Path:
    product_path = Path(product_text)
    if product_path.is_absolute():
        return product_path

    data_product_path = PROJECT_DIR / "data" / product_text
    return data_product_path if data_product_path.exists() else PROJECT_DIR / product_path


def _resolve_explicit_bands(args: argparse.Namespace) -> SentinelBandPaths | None:
    values = {
        "blue": args.blue,
        "green": args.green,
        "red": args.red,
        "nir": args.nir,
        "swir1": args.swir1,
        "swir2": args.swir2,
    }
    if not any(values.values()):
        return None
    if not values["red"] or not values["nir"]:
        raise VegetationError("При явном выборе файлов обязательны оба канала: --red и --nir.")

    optional_names = ("blue", "green", "swir1", "swir2")
    supplied_optional = [name for name in optional_names if values[name]]
    if supplied_optional and len(supplied_optional) != len(optional_names):
        missing = ", ".join(name for name in optional_names if not values[name])
        raise VegetationError(
            "Для полной классификации укажите B02, B03, B11 и B12. "
            f"Отсутствуют аргументы: {missing}"
        )

    return SentinelBandPaths(
        blue=project_path(values["blue"]) if values["blue"] else None,
        green=project_path(values["green"]) if values["green"] else None,
        red=project_path(values["red"]),
        nir=project_path(values["nir"]),
        swir1=project_path(values["swir1"]) if values["swir1"] else None,
        swir2=project_path(values["swir2"]) if values["swir2"] else None,
        calibration=(
            modern_l2a_calibration("CLI option --apply-boa-offset")
            if args.apply_boa_offset
            else None
        ),
    )


def resolve_input_bands(args: argparse.Namespace) -> SentinelBandPaths:
    explicit = _resolve_explicit_bands(args)
    if explicit is not None:
        return explicit

    if args.product:
        return discover_sentinel_bands(resolve_product_path(args.product))
    return discover_default_bands(PROJECT_DIR / "data")


def main() -> int:
    configure_terminal_encoding()
    args = parse_args()
    try:
        band_paths = resolve_input_bands(args)
        result = run_pipeline(
            band_paths,
            out_dir=project_path(args.out),
            processed_dir=project_path(args.processed),
            progress=print,
        )
        mode_name = (
            "полная классификация поверхности"
            if result.mode == "multispectral"
            else "классификация растительности по NDVI"
        )
        print(f"Готово: {mode_name}.")
        print(f"Результаты: {project_path(args.out)}")
        print()
        print(result.summary_text)
        return 0
    except VegetationError as error:
        print(f"Ошибка: {error}", file=sys.stderr)
        return 1
    except Exception as error:
        print(f"Непредвиденная ошибка: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
