from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

import numpy as np


PROJECT_DIR = Path(__file__).resolve().parent
PLANETARY_COMPUTER_STAC = "https://planetarycomputer.microsoft.com/api/stac/v1"


def project_path(path_text: str) -> Path:
    path = Path(path_text)
    if path.is_absolute():
        return path
    return PROJECT_DIR / path


def require_rasterio():
    try:
        import rasterio
        from rasterio.transform import from_origin
    except ImportError as error:
        raise RuntimeError(
            "Для создания демонстрационных GeoTIFF нужна библиотека rasterio. "
            "Установите зависимости командой: python -m pip install -r requirements.txt"
        ) from error
    return rasterio, from_origin


def create_demo_data(out_dir: Path, width: int = 220, height: int = 180) -> None:
    """Создает небольшие учебные B04.tif и B08.tif без скачивания из интернета."""
    rasterio, from_origin = require_rasterio()
    out_dir.mkdir(parents=True, exist_ok=True)

    y, x = np.indices((height, width))
    ndvi_model = np.full((height, width), 0.12, dtype=np.float32)

    # Зоны сделаны так, чтобы на карте были все четыре класса NDVI.
    ndvi_model[:, : width // 4] = 0.05
    ndvi_model[:, width // 4 : width // 2] = 0.30
    ndvi_model[:, width // 2 : 3 * width // 4] = 0.50
    ndvi_model[:, 3 * width // 4 :] = 0.72

    lake = ((x - width * 0.20) ** 2 / (width * 0.13) ** 2) + (
        (y - height * 0.35) ** 2 / (height * 0.18) ** 2
    ) < 1
    dense_park = ((x - width * 0.78) ** 2 / (width * 0.15) ** 2) + (
        (y - height * 0.55) ** 2 / (height * 0.25) ** 2
    ) < 1
    ndvi_model[lake] = -0.08
    ndvi_model[dense_park] = 0.78

    rng = np.random.default_rng(42)
    ndvi_model += rng.normal(0, 0.035, size=ndvi_model.shape).astype(np.float32)
    ndvi_model = np.clip(ndvi_model, -0.2, 0.9)

    total_reflectance = 6200.0 + 350.0 * np.sin(x / 20.0)
    nir = ((1 + ndvi_model) * total_reflectance / 2).astype(np.uint16)
    red = ((1 - ndvi_model) * total_reflectance / 2).astype(np.uint16)

    profile = {
        "driver": "GTiff",
        "height": height,
        "width": width,
        "count": 1,
        "dtype": "uint16",
        "crs": "EPSG:4326",
        "transform": from_origin(30.20, 60.05, 0.00012, 0.00012),
        "compress": "lzw",
    }

    for name, array in {"B04.tif": red, "B08.tif": nir}.items():
        with rasterio.open(out_dir / name, "w", **profile) as dataset:
            dataset.write(array, 1)

    metadata = {
        "source": "synthetic demo",
        "description": "Учебные GeoTIFF-файлы с искусственными зонами NDVI.",
        "files": ["B04.tif", "B08.tif"],
    }
    (out_dir / "demo_metadata.json").write_text(
        json.dumps(metadata, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    print(f"Демонстрационные файлы созданы в: {out_dir}")


def save_cog_window(asset_url: str, destination: Path, bbox: list[float]) -> None:
    """Сохраняет из COG только небольшой фрагмент, заданный bbox."""
    try:
        import rasterio
        from rasterio.warp import transform_bounds
        from rasterio.windows import from_bounds
    except ImportError as error:
        raise RuntimeError(
            "Для чтения фрагмента Sentinel-2 нужна библиотека rasterio. "
            "Установите зависимости командой: python -m pip install -r requirements.txt"
        ) from error

    destination.parent.mkdir(parents=True, exist_ok=True)
    print(f"Сохранение фрагмента {destination.name}...")

    with rasterio.open(asset_url) as source:
        left, bottom, right, top = transform_bounds("EPSG:4326", source.crs, *bbox, densify_pts=21)
        window = from_bounds(left, bottom, right, top, transform=source.transform)
        window = window.round_offsets().round_lengths()

        if window.width <= 0 or window.height <= 0:
            raise RuntimeError("Указанный bbox не пересекается со сценой Sentinel-2.")

        data = source.read(1, window=window, masked=True)
        profile = source.profile.copy()
        profile.update(
            {
                "height": data.shape[0],
                "width": data.shape[1],
                "transform": source.window_transform(window),
                "count": 1,
                "compress": "lzw",
            }
        )

        output = data.filled(source.nodata or 0)
        with rasterio.open(destination, "w", **profile) as dataset:
            dataset.write(output, 1)


def download_from_planetary_computer(
    out_dir: Path,
    bbox: list[float],
    datetime_range: str,
    cloud_cover: float,
) -> None:
    """Ищет подходящую сцену Sentinel-2 L2A в STAC и скачивает каналы B04/B08."""
    try:
        import planetary_computer as pc
        from pystac_client import Client
    except ImportError as error:
        raise RuntimeError(
            "Для автоматического скачивания нужны pystac-client и planetary-computer. "
            "Установите зависимости командой: python -m pip install -r requirements.txt"
        ) from error

    catalog = Client.open(PLANETARY_COMPUTER_STAC)
    search = catalog.search(
        collections=["sentinel-2-l2a"],
        bbox=bbox,
        datetime=datetime_range,
        query={"eo:cloud_cover": {"lt": cloud_cover}},
        limit=20,
    )
    items = list(search.items())
    if not items:
        raise RuntimeError(
            "Не найдено сцен Sentinel-2 для заданной области и дат. "
            "Попробуйте расширить диапазон дат или увеличить допустимую облачность."
        )

    items.sort(key=lambda item: item.properties.get("eo:cloud_cover", 100.0))
    item = pc.sign(items[0])
    print(f"Выбрана сцена: {item.id}")
    print(f"Облачность: {item.properties.get('eo:cloud_cover', 'нет данных')}%")

    for asset_key, filename in {"B04": "B04.tif", "B08": "B08.tif"}.items():
        if asset_key not in item.assets:
            raise RuntimeError(f"В найденной сцене нет канала {asset_key}.")
        save_cog_window(item.assets[asset_key].href, out_dir / filename, bbox)

    print(f"Файлы Sentinel-2 сохранены в: {out_dir}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Скачивание каналов Sentinel-2 B04/B08 или создание демонстрационных данных."
    )
    parser.add_argument(
        "--out",
        default="data/raw",
        help="Папка для файлов B04.tif и B08.tif. По умолчанию: data/raw",
    )
    parser.add_argument(
        "--demo",
        action="store_true",
        help="Создать искусственные учебные GeoTIFF без интернета.",
    )
    parser.add_argument(
        "--bbox",
        nargs=4,
        type=float,
        metavar=("MIN_LON", "MIN_LAT", "MAX_LON", "MAX_LAT"),
        default=[30.20, 59.96, 30.38, 60.05],
        help="Границы области поиска. По умолчанию: участок Санкт-Петербурга.",
    )
    parser.add_argument(
        "--datetime",
        default="2024-06-01/2024-08-31",
        help="Диапазон дат STAC, например 2024-06-01/2024-08-31.",
    )
    parser.add_argument(
        "--cloud-cover",
        type=float,
        default=20.0,
        help="Максимальная облачность в процентах. По умолчанию: 20.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    out_dir = project_path(args.out)

    try:
        if args.demo:
            create_demo_data(out_dir)
        else:
            download_from_planetary_computer(
                out_dir=out_dir,
                bbox=args.bbox,
                datetime_range=args.datetime,
                cloud_cover=args.cloud_cover,
            )
        return 0
    except Exception as error:
        print(f"Ошибка загрузки данных: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
