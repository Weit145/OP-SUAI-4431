from __future__ import annotations

import numpy as np

from src.processing.indices import calculate_spectral_indices, normalized_difference


def test_normalized_difference_masks_invalid_divisions_and_pixels() -> None:
    first = np.array([[3.0, 1.0, np.nan, 4.0]], dtype="float32")
    second = np.array([[1.0, -1.0, 2.0, 2.0]], dtype="float32")
    valid_mask = np.array([[True, True, True, False]])

    result = normalized_difference(first, second, valid_mask)

    assert result.dtype == np.dtype("float32")
    np.testing.assert_allclose(result[0, 0], 0.5)
    assert np.isnan(result[0, 1:]).all()


def test_calculate_spectral_indices_uses_documented_formulas() -> None:
    indices = calculate_spectral_indices(
        blue=np.array([[1.0]], dtype="float32"),
        green=np.array([[2.0]], dtype="float32"),
        red=np.array([[3.0]], dtype="float32"),
        nir=np.array([[7.0]], dtype="float32"),
        swir1=np.array([[5.0]], dtype="float32"),
        swir2=np.array([[9.0]], dtype="float32"),
    )

    np.testing.assert_allclose(indices.ndvi, [[4.0 / 10.0]])
    np.testing.assert_allclose(indices.ndwi, [[-5.0 / 9.0]])
    np.testing.assert_allclose(indices.mndwi, [[-3.0 / 7.0]])
    np.testing.assert_allclose(indices.ndbi, [[-2.0 / 12.0]])
    np.testing.assert_allclose(indices.bsi, [[0.0]])
    np.testing.assert_allclose(indices.urban_index, [[2.0 / 16.0]])
    np.testing.assert_array_equal(indices.ui, indices.urban_index)


def test_calculate_spectral_indices_propagates_nan_and_valid_mask() -> None:
    shape = (1, 3)
    bands = {
        "blue": np.ones(shape, dtype="float32"),
        "green": np.full(shape, 2.0, dtype="float32"),
        "red": np.full(shape, 3.0, dtype="float32"),
        "nir": np.full(shape, 7.0, dtype="float32"),
        "swir1": np.full(shape, 5.0, dtype="float32"),
        "swir2": np.full(shape, 9.0, dtype="float32"),
    }
    bands["blue"][0, 1] = np.nan

    indices = calculate_spectral_indices(
        **bands,
        valid_mask=np.array([[True, True, False]]),
    )

    for array in indices.as_dict().values():
        assert np.isfinite(array[0, 0])
        assert np.isnan(array[0, 1:]).all()
