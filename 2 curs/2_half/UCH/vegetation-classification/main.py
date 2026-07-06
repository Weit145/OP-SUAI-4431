from __future__ import annotations

import argparse
import sys
from pathlib import Path

from src.classification import calculate_statistics, classify_ndvi
from src.ndvi import calculate_ndvi
from src.utils import (
    VegetationError,
    find_bands_in_product,
    find_default_bands,
    load_bands,
    save_geotiff,
    write_summary,
)
from src.visualization import save_classified_map, save_ndvi_map


PROJECT_DIR = Path(__file__).resolve().parent


def configure_terminal_encoding() -> None:
    """Делает русский текст читаемым в терминале Windows и VS Code."""
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    if hasattr(sys.stderr, "reconfigure"):
        sys.stderr.reconfigure(encoding="utf-8")


def project_path(path_text: str) -> Path:
    """Возвращает абсолютный путь относительно папки проекта."""
    path = Path(path_text)
    if path.is_absolute():
        return path
    return PROJECT_DIR / path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Классификация растительности по индексу NDVI для каналов Sentinel-2 B04 и B08."
    )
    parser.add_argument(
        "product_name",
        nargs="?",
        help="Название папки продукта Sentinel-2 внутри data, например L2A_T35VMF_A009487_20260630T095026.",
    )
    parser.add_argument(
        "--product",
        help="Название или путь к папке продукта Sentinel-2. Ищутся каналы B04/B08 внутри этой папки.",
    )
    parser.add_argument(
        "--red",
        default=None,
        help="Путь к красному каналу Sentinel-2 B04. Если не указан, программа ищет канал автоматически.",
    )
    parser.add_argument(
        "--nir",
        default=None,
        help="Путь к ближнему инфракрасному каналу Sentinel-2 B08. Если не указан, программа ищет канал автоматически.",
    )
    parser.add_argument(
        "--out",
        default="results",
        help="Папка для PNG-карт, CSV-таблицы и текстового отчета. По умолчанию: results",
    )
    parser.add_argument(
        "--processed",
        default="data/processed",
        help="Папка для промежуточных GeoTIFF-файлов. По умолчанию: data/processed",
    )
    return parser.parse_args()


def resolve_product_path(product_text: str) -> Path:
    """Определяет путь к папке продукта по ее названию или полному пути."""
    product_path = Path(product_text)
    if product_path.is_absolute():
        return product_path

    data_product_path = PROJECT_DIR / "data" / product_text
    if data_product_path.exists():
        return data_product_path

    return PROJECT_DIR / product_path


def resolve_input_bands(
    red_arg: str | None,
    nir_arg: str | None,
    product_arg: str | None,
    product_name: str | None,
) -> tuple[Path, Path]:
    """Определяет, какие файлы B04 и B08 нужно использовать."""
    if red_arg or nir_arg:
        if not red_arg or not nir_arg:
            raise VegetationError("Если указан один канал, нужно указать оба: --red и --nir.")
        return project_path(red_arg), project_path(nir_arg)

    selected_product = product_arg or product_name
    if selected_product:
        return find_bands_in_product(resolve_product_path(selected_product))

    return find_default_bands(PROJECT_DIR / "data")


def run_pipeline(red_path: Path, nir_path: Path, out_dir: Path, processed_dir: Path) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)
    processed_dir.mkdir(parents=True, exist_ok=True)

    print("Загрузка каналов Sentinel-2...")
    red, nir, valid_mask, profile = load_bands(red_path, nir_path)

    print("Расчет NDVI...")
    ndvi = calculate_ndvi(red, nir, valid_mask)

    print("Классификация растительности...")
    classified = classify_ndvi(ndvi, valid_mask)

    print("Расчет статистики классов...")
    statistics = calculate_statistics(classified)

    print("Сохранение результатов...")
    statistics_path = out_dir / "class_statistics.csv"
    summary_path = out_dir / "result_summary.txt"
    ndvi_png_path = out_dir / "ndvi_map.png"
    classified_png_path = out_dir / "classified_map.png"

    statistics.to_csv(statistics_path, index=False, encoding="utf-8-sig")
    summary_text = write_summary(summary_path, red_path, nir_path, statistics)
    save_ndvi_map(ndvi, ndvi_png_path)
    save_classified_map(classified, classified_png_path)

    save_geotiff(processed_dir / "ndvi.tif", ndvi, profile, dtype="float32", nodata=-9999.0)
    save_geotiff(
        processed_dir / "classified_vegetation.tif",
        classified,
        profile,
        dtype="uint8",
        nodata=0,
    )

    print(f"Готово. Результаты сохранены в: {out_dir}")
    print(f"Таблица статистики: {statistics_path}")
    print(f"Краткий отчет: {summary_path}")
    print()
    print(summary_text)


def main() -> int:
    configure_terminal_encoding()
    args = parse_args()
    try:
        red_path, nir_path = resolve_input_bands(
            args.red,
            args.nir,
            args.product,
            args.product_name,
        )
        run_pipeline(
            red_path=red_path,
            nir_path=nir_path,
            out_dir=project_path(args.out),
            processed_dir=project_path(args.processed),
        )
        return 0
    except VegetationError as error:
        print(f"Ошибка: {error}", file=sys.stderr)
        return 1
    except Exception as error:
        print(f"Непредвиденная ошибка: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
