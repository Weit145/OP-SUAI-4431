from __future__ import annotations

from dataclasses import dataclass

import numpy as np


@dataclass(frozen=True, slots=True)
class SpectralIndices:
    ndvi: np.ndarray
    ndwi: np.ndarray
    mndwi: np.ndarray
    ndbi: np.ndarray
    bsi: np.ndarray
    urban_index: np.ndarray

    def as_dict(self) -> dict[str, np.ndarray]:
        return {
            "ndvi": self.ndvi,
            "ndwi": self.ndwi,
            "mndwi": self.mndwi,
            "ndbi": self.ndbi,
            "bsi": self.bsi,
            "urban_index": self.urban_index,
        }


def normalized_difference(
    first: np.ndarray,
    second: np.ndarray,
    valid_mask: np.ndarray | None = None,
) -> np.ndarray:
    first = np.asarray(first, dtype="float32")
    second = np.asarray(second, dtype="float32")
    valid = np.isfinite(first) & np.isfinite(second) & ((first + second) != 0)
    if valid_mask is not None:
        valid &= np.asarray(valid_mask, dtype=bool)

    with np.errstate(divide="ignore", invalid="ignore"):
        result = (first - second) / (first + second)
    result[~valid] = np.nan
    return np.clip(result, -1.0, 1.0)


def calculate_ndvi(
    red: np.ndarray,
    nir: np.ndarray,
    valid_mask: np.ndarray | None = None,
) -> np.ndarray:
    return normalized_difference(nir, red, valid_mask)


def calculate_spectral_indices(
    blue: np.ndarray,
    green: np.ndarray,
    red: np.ndarray,
    nir: np.ndarray,
    swir1: np.ndarray,
    swir2: np.ndarray,
    valid_mask: np.ndarray | None = None,
) -> SpectralIndices:
    bands = (blue, green, red, nir, swir1, swir2)
    valid = np.ones(red.shape, dtype=bool)
    for band in bands:
        valid &= np.isfinite(band)
    if valid_mask is not None:
        valid &= np.asarray(valid_mask, dtype=bool)

    return SpectralIndices(
        ndvi=calculate_ndvi(red, nir, valid),
        ndwi=normalized_difference(green, nir, valid),
        mndwi=normalized_difference(green, swir1, valid),
        ndbi=normalized_difference(swir1, nir, valid),
        bsi=normalized_difference(swir1 + red, nir + blue, valid),
        urban_index=normalized_difference(swir2, nir, valid),
    )
