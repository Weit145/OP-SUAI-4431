from __future__ import annotations

import numpy as np
from src.processing.classification import (
    calculate_land_cover_statistics,
    classify_land_cover,
)
from src.processing.indices import SpectralIndices


def test_classify_land_cover_assigns_every_supported_class_with_priority() -> None:
    indices = SpectralIndices(
        **{
            # Water, soil, built-up, sparse, moderate, dense, other, masked.
            "ndvi": np.array([[0.1, 0.1, 0.1, 0.2], [0.4, 0.6, 0.1, 0.1]]),
            "ndwi": np.array([[0.2, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.2]]),
            "mndwi": np.array([[0.2, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.2]]),
            "ndbi": np.array([[0.3, 0.0, 0.3, 0.0], [0.0, 0.0, 0.0, 0.3]]),
            "bsi": np.array([[0.2, 0.3, 0.2, 0.0], [0.0, 0.0, 0.0, 0.2]]),
            "urban_index": np.array([[0.3, -0.1, 0.2, 0.0], [0.0, 0.0, 0.0, 0.3]]),
        }
    )
    valid_mask = np.array([[True, True, True, True], [True, True, True, False]])

    classified = classify_land_cover(indices, valid_mask)

    # The first pixel also matches built-up/bare-soil inputs, but water wins.
    expected = np.array([[1, 2, 3, 4], [5, 6, 7, 0]], dtype="uint8")
    np.testing.assert_array_equal(classified, expected)


def test_classify_land_cover_treats_nan_as_nodata() -> None:
    values = np.zeros((1, 2), dtype="float32")
    indices = SpectralIndices(
        **{
            "ndvi": values.copy(),
            "ndwi": values.copy(),
            "mndwi": values.copy(),
            "ndbi": values.copy(),
            "bsi": values.copy(),
            "urban_index": values.copy(),
        }
    )
    indices.mndwi[0, 1] = np.nan

    classified = classify_land_cover(indices)

    np.testing.assert_array_equal(classified, [[7, 0]])


def test_land_cover_statistics_exclude_nodata_and_include_empty_classes() -> None:
    classified = np.array(
        [
            [0, 1, 1, 2],
            [3, 4, 5, 6],
            [7, 7, 7, 0],
        ],
        dtype="uint8",
    )

    statistics = calculate_land_cover_statistics(classified).set_index("class_id")

    assert list(statistics.index) == [1, 2, 3, 4, 5, 6, 7]
    assert statistics["pixel_count"].to_dict() == {
        1: 2,
        2: 1,
        3: 1,
        4: 1,
        5: 1,
        6: 1,
        7: 3,
    }
    assert statistics["percent"].to_dict() == {
        1: 20.0,
        2: 10.0,
        3: 10.0,
        4: 10.0,
        5: 10.0,
        6: 10.0,
        7: 30.0,
    }
