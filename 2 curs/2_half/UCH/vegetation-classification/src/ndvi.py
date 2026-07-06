from __future__ import annotations

import numpy as np


def calculate_ndvi(red: np.ndarray, nir: np.ndarray, valid_mask: np.ndarray | None = None) -> np.ndarray:
    """Рассчитывает NDVI по формуле (B08 - B04) / (B08 + B04)."""
    red = red.astype("float32", copy=False)
    nir = nir.astype("float32", copy=False)

    denominator = nir + red
    numerator = nir - red

    # np.divide позволяет аккуратно обработать деление на ноль.
    ndvi = np.full(red.shape, np.nan, dtype="float32")
    can_divide = denominator != 0
    if valid_mask is not None:
        can_divide &= valid_mask

    np.divide(numerator, denominator, out=ndvi, where=can_divide)
    ndvi = np.clip(ndvi, -1.0, 1.0)
    ndvi[~np.isfinite(ndvi)] = np.nan
    return ndvi
