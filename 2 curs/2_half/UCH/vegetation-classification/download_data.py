from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any

import numpy as np


PROJECT_DIR = Path(__file__).resolve().parent
PLANETARY_COMPUTER_STAC = "https://planetarycomputer.microsoft.com/api/stac/v1"
SPECTRAL_BANDS = ("B02", "B03", "B04", "B08", "B11", "B12")
OPTIONAL_MASK_BANDS = ("SCL",)
REFLECTANCE_SCALE = 10_000

# Representative surface-reflectance values. They are deliberately separated
# from the classifier thresholds so that the demo remains stable after adding
# a small amount of texture.
DEMO_SURFACES: tuple[dict[str, Any], ...] = (
    {
        "class_id": 1,
        "key": "water",
        "name": "Вода",
        "layout": "верхняя левая зона",
        "reflectance": {
            "B02": 0.12,
            "B03": 0.18,
            "B04": 0.10,
            "B08": 0.04,
            "B11": 0.025,
            "B12": 0.02,
        },
    },
    {
        "class_id": 2,
        "key": "bare_soil",
        "name": "Открытая почва",
        "layout": "верхняя центральная зона",
        "reflectance": {
            "B02": 0.12,
            "B03": 0.16,
            "B04": 0.24,
            "B08": 0.28,
            "B11": 0.38,
            "B12": 0.34,
        },
    },
    {
        "class_id": 3,
        "key": "built_up",
        "name": "Застройка",
        "layout": "верхняя правая зона",
        "reflectance": {
            "B02": 0.15,
            "B03": 0.17,
            "B04": 0.20,
            "B08": 0.24,
            "B11": 0.38,
            "B12": 0.42,
        },
    },
    {
        "class_id": 4,
        "key": "sparse_vegetation",
        "name": "Слабая растительность",
        "layout": "нижняя левая зона",
        "reflectance": {
            "B02": 0.10,
            "B03": 0.15,
            "B04": 0.14,
            "B08": 0.26,
            "B11": 0.20,
            "B12": 0.15,
        },
    },
    {
        "class_id": 5,
        "key": "moderate_vegetation",
        "name": "Средняя растительность",
        "layout": "нижняя центральная зона",
        "reflectance": {
            "B02": 0.08,
            "B03": 0.14,
            "B04": 0.10,
            "B08": 0.30,
            "B11": 0.17,
            "B12": 0.12,
        },
    },
    {
        "class_id": 6,
        "key": "dense_vegetation",
        "name": "Густая растительность",
        "layout": "нижняя правая зона",
        "reflectance": {
            "B02": 0.06,
            "B03": 0.12,
            "B04": 0.06,
            "B08": 0.36,
            "B11": 0.12,
            "B12": 0.08,
        },
    },
)


def configure_terminal_encoding() -> None:
    """Keep Russian CLI output readable in Windows terminals."""

    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    if hasattr(sys.stderr, "reconfigure"):
        sys.stderr.reconfigure(encoding="utf-8")


def project_path(path_text: str) -> Path:
    """Resolve a CLI path relative to the project directory."""
    path = Path(path_text)
    if path.is_absolute():
        return path
    return PROJECT_DIR / path


def require_rasterio():
    """Import Rasterio lazily to keep ``--help`` available without extras."""
    try:
        import rasterio
        from rasterio.transform import from_origin
    except ImportError as error:
        raise RuntimeError(
            "Для работы с GeoTIFF нужна библиотека rasterio. "
            "Установите зависимости командой: poetry install"
        ) from error
    return rasterio, from_origin


def _demo_zone_map(width: int, height: int) -> np.ndarray:
    """Return six contiguous zones arranged as two rows by three columns."""
    y, x = np.indices((height, width))

    # A slightly curved row boundary makes the synthetic image less artificial
    # while preserving large, easy-to-recognize regions for every class.
    row_boundary = height / 2 + np.sin(x / 15.0) * height * 0.035
    row = (y >= row_boundary).astype("uint8")
    column = np.minimum(x * 3 // width, 2).astype("uint8")
    return row * 3 + column + 1


def _create_demo_bands(
    width: int,
    height: int,
    *,
    seed: int = 42,
) -> tuple[dict[str, np.ndarray], np.ndarray]:
    """Build co-registered uint16 bands with realistic scaled reflectance."""
    zones = _demo_zone_map(width, height)
    y, x = np.indices((height, width))

    # One illumination field is shared by every band, so it adds visible
    # texture without changing normalized-difference indices substantially.
    illumination = (
        1.0 + 0.035 * np.sin(x / 19.0) + 0.025 * np.cos(y / 13.0) + 0.015 * np.sin((x + y) / 29.0)
    ).astype("float32")
    rng = np.random.default_rng(seed)

    bands: dict[str, np.ndarray] = {}
    for band_code in SPECTRAL_BANDS:
        reflectance = np.zeros((height, width), dtype="float32")
        for surface in DEMO_SURFACES:
            reflectance[zones == surface["class_id"]] = surface["reflectance"][band_code]

        independent_texture = rng.normal(0.0, 0.001, size=(height, width)).astype("float32")
        digital_numbers = (reflectance * illumination + independent_texture) * REFLECTANCE_SCALE
        bands[band_code] = np.clip(np.rint(digital_numbers), 1, REFLECTANCE_SCALE).astype("uint16")

    return bands, zones


def _remove_stale_files(out_dir: Path, names: tuple[str, ...]) -> None:
    for name in names:
        (out_dir / name).unlink(missing_ok=True)


def create_demo_data(out_dir: Path, width: int = 240, height: int = 160) -> None:
    """Create six small, co-registered multispectral demonstration GeoTIFFs."""
    rasterio, from_origin = require_rasterio()
    out_dir.mkdir(parents=True, exist_ok=True)
    _remove_stale_files(out_dir, ("SCL.tif", "scene_metadata.json"))
    bands, zones = _create_demo_bands(width, height)

    profile = {
        "driver": "GTiff",
        "height": height,
        "width": width,
        "count": 1,
        "dtype": "uint16",
        "crs": "EPSG:32636",
        "transform": from_origin(340_000, 6_660_000, 10, 10),
        "nodata": 0,
        "compress": "lzw",
    }

    for band_code, array in bands.items():
        destination = out_dir / f"{band_code}.tif"
        with rasterio.open(destination, "w", **profile) as dataset:
            dataset.write(array, 1)
            dataset.set_band_description(1, band_code)
            dataset.update_tags(
                source="synthetic demo",
                band=band_code,
                reflectance_scale=str(REFLECTANCE_SCALE),
            )

    zone_counts = {
        str(class_id): int(np.count_nonzero(zones == class_id))
        for class_id in range(1, len(DEMO_SURFACES) + 1)
    }
    metadata = {
        "source": "synthetic demo",
        "description": (
            "Согласованные учебные каналы Sentinel-2 с зонами воды, открытой почвы, "
            "застройки и трёх уровней растительности."
        ),
        "files": [f"{band_code}.tif" for band_code in SPECTRAL_BANDS],
        "bands": list(SPECTRAL_BANDS),
        "reflectance_scale": REFLECTANCE_SCALE,
        "grid": {
            "width": width,
            "height": height,
            "crs": profile["crs"],
            "pixel_size_metres": 10,
            "nodata": profile["nodata"],
        },
        "zones": [
            {
                "class_id": surface["class_id"],
                "key": surface["key"],
                "name": surface["name"],
                "layout": surface["layout"],
                "pixel_count": zone_counts[str(surface["class_id"])],
                "reference_reflectance": surface["reflectance"],
            }
            for surface in DEMO_SURFACES
        ],
    }
    (out_dir / "demo_metadata.json").write_text(
        json.dumps(metadata, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    print(f"Демонстрационные файлы созданы в: {out_dir}")


def save_cog_window(
    asset_url: str,
    destination: Path,
    bbox: list[float],
    *,
    calibration: tuple[float, float] | None = None,
) -> None:
    """Save only the requested WGS84 bounding-box window from a remote COG."""
    try:
        import rasterio
        from rasterio.warp import transform_bounds
        from rasterio.windows import Window, from_bounds
    except ImportError as error:
        raise RuntimeError(
            "Для чтения фрагмента Sentinel-2 нужна библиотека rasterio. "
            "Установите зависимости командой: poetry install"
        ) from error

    destination.parent.mkdir(parents=True, exist_ok=True)
    print(f"Сохранение фрагмента {destination.name}...")

    with rasterio.open(asset_url) as source:
        left, bottom, right, top = transform_bounds(
            "EPSG:4326",
            source.crs,
            *bbox,
            densify_pts=21,
        )
        requested = from_bounds(left, bottom, right, top, transform=source.transform)
        requested = requested.round_offsets().round_lengths()
        raster_extent = Window(0, 0, source.width, source.height)

        try:
            window = requested.intersection(raster_extent)
        except Exception as error:
            raise RuntimeError("Указанный bbox не пересекается со сценой Sentinel-2.") from error

        if window.width <= 0 or window.height <= 0:
            raise RuntimeError("Указанный bbox не пересекается со сценой Sentinel-2.")

        data = source.read(1, window=window, masked=True)
        profile = source.profile.copy()
        profile.update(
            height=data.shape[0],
            width=data.shape[1],
            transform=source.window_transform(window),
            count=1,
            compress="lzw",
        )

        effective_calibration = calibration
        if effective_calibration is None:
            embedded_scale = float(source.scales[0]) if source.scales else 1.0
            embedded_offset = float(source.offsets[0]) if source.offsets else 0.0
            if embedded_scale != 1.0 or embedded_offset != 0.0:
                effective_calibration = embedded_scale, embedded_offset

        if effective_calibration is None:
            output = data.filled(source.nodata if source.nodata is not None else 0)
        else:
            scale, offset = effective_calibration
            raw = np.asarray(data.data, dtype="float32")
            invalid = np.ma.getmaskarray(data) | (raw == 0)
            output = raw * scale + offset
            output[invalid] = -9999.0
            profile.update(dtype="float32", nodata=-9999.0)
        with rasterio.open(destination, "w", **profile) as dataset:
            dataset.write(output, 1)


def _processing_baseline(item: Any) -> tuple[int, int] | None:
    value = str(item.properties.get("s2:processing_baseline", ""))
    match = re.search(r"(\d{2})[._]?(\d{2})", value)
    return (int(match.group(1)), int(match.group(2))) if match else None


def _asset_calibration(asset: Any, item: Any) -> tuple[float, float] | None:
    raster_bands = asset.extra_fields.get("raster:bands", [])
    if raster_bands:
        band_metadata = raster_bands[0]
        if "scale" in band_metadata or "offset" in band_metadata:
            return (
                float(band_metadata.get("scale", 1.0)),
                float(band_metadata.get("offset", 0.0)),
            )

    baseline = _processing_baseline(item)
    if baseline is not None and baseline >= (4, 0):
        return 1.0 / REFLECTANCE_SCALE, -1_000.0 / REFLECTANCE_SCALE
    return None


def _save_scene_metadata(
    out_dir: Path,
    item: Any,
    bbox: list[float],
    datetime_range: str,
) -> None:
    """Store enough STAC information to identify the downloaded source scene."""
    saved_bands = [
        band_code
        for band_code in (*SPECTRAL_BANDS, *OPTIONAL_MASK_BANDS)
        if band_code in item.assets
    ]
    metadata = {
        "source": "Microsoft Planetary Computer / Sentinel-2 L2A",
        "scene_id": item.id,
        "datetime": item.properties.get("datetime"),
        "requested_datetime_range": datetime_range,
        "cloud_cover_percent": item.properties.get("eo:cloud_cover"),
        "bbox_wgs84": bbox,
        "files": [f"{band_code}.tif" for band_code in saved_bands],
        "bands": saved_bands,
    }
    (out_dir / "scene_metadata.json").write_text(
        json.dumps(metadata, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )


def download_from_planetary_computer(
    out_dir: Path,
    bbox: list[float],
    datetime_range: str,
    cloud_cover: float,
) -> None:
    """Download six spectral bands from the clearest matching Sentinel-2 scene."""
    try:
        import planetary_computer as pc
        from pystac_client import Client
    except ImportError as error:
        raise RuntimeError(
            "Для автоматического скачивания нужны pystac-client и planetary-computer. "
            "Установите зависимости командой: poetry install"
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
            "Расширьте диапазон дат или увеличьте допустимую облачность."
        )

    items.sort(key=lambda item: item.properties.get("eo:cloud_cover", 100.0))
    item = pc.sign(items[0])
    print(f"Выбрана сцена: {item.id}")
    print(f"Облачность: {item.properties.get('eo:cloud_cover', 'нет данных')}%")

    missing_bands = [band_code for band_code in SPECTRAL_BANDS if band_code not in item.assets]
    if missing_bands:
        raise RuntimeError("В найденной сцене отсутствуют каналы: " + ", ".join(missing_bands))

    out_dir.mkdir(parents=True, exist_ok=True)
    _remove_stale_files(out_dir, ("SCL.tif", "demo_metadata.json"))

    for band_code in SPECTRAL_BANDS:
        asset = item.assets[band_code]
        save_cog_window(
            asset.href,
            out_dir / f"{band_code}.tif",
            bbox,
            calibration=_asset_calibration(asset, item),
        )

    for band_code in OPTIONAL_MASK_BANDS:
        if band_code in item.assets:
            save_cog_window(
                item.assets[band_code].href,
                out_dir / f"{band_code}.tif",
                bbox,
            )

    _save_scene_metadata(out_dir, item, bbox, datetime_range)
    print(f"Файлы Sentinel-2 сохранены в: {out_dir}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Скачивание каналов Sentinel-2 B02, B03, B04, B08, B11, B12 "
            "или создание демонстрационных данных."
        )
    )
    parser.add_argument(
        "--out",
        default="data/raw",
        help="Папка для шести GeoTIFF. По умолчанию: data/raw",
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
    configure_terminal_encoding()
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
