from __future__ import annotations

import math
from pathlib import Path
from typing import Any, Sequence

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import BoundaryNorm, ListedColormap
from matplotlib.patches import Patch

from src.processing.classification import VEGETATION_CLASSES


def _preview(array: np.ndarray, max_side: int = 2_000) -> np.ndarray:
    """Downsample a large raster only for PNG rendering."""

    step = max(1, math.ceil(max(array.shape) / max_side))
    return array[::step, ::step]


def save_index_map(
    index: np.ndarray,
    output_path: Path,
    *,
    title: str,
    label: str,
    cmap: str = "RdYlGn",
) -> None:
    """Save a spectral index as a compact PNG preview."""

    output_path.parent.mkdir(parents=True, exist_ok=True)

    fig, ax = plt.subplots(figsize=(9, 7), dpi=150)
    image = ax.imshow(_preview(index), cmap=cmap, vmin=-1, vmax=1)
    ax.set_title(title)
    ax.set_axis_off()

    colorbar = fig.colorbar(image, ax=ax, fraction=0.046, pad=0.04)
    colorbar.set_label(label)

    fig.tight_layout()
    fig.savefig(output_path, bbox_inches="tight")
    plt.close(fig)


def save_ndvi_map(ndvi: np.ndarray, output_path: Path) -> None:
    """Save the NDVI preview using the conventional red-to-green palette."""

    save_index_map(ndvi, output_path, title="Карта NDVI", label="NDVI")


def save_classified_map(
    classified: np.ndarray,
    output_path: Path,
    *,
    classes: Sequence[Any] = VEGETATION_CLASSES,
    title: str = "Классификация растительности по NDVI",
) -> None:
    """Save a classified raster with a legend assembled from class metadata."""

    output_path.parent.mkdir(parents=True, exist_ok=True)

    colors = [item.color for item in classes]
    cmap = ListedColormap(colors)
    boundaries = np.arange(0.5, len(classes) + 1.5, 1.0)
    norm = BoundaryNorm(boundaries, cmap.N)
    masked = np.ma.masked_where(classified == 0, classified)

    fig, ax = plt.subplots(figsize=(9, 7), dpi=150)
    ax.imshow(_preview(masked), cmap=cmap, norm=norm)
    ax.set_title(title)
    ax.set_axis_off()

    legend_items = [
        Patch(facecolor=item.color, edgecolor="black", label=f"{item.class_id}. {item.name}")
        for item in classes
    ]
    ax.legend(
        handles=legend_items,
        loc="lower center",
        bbox_to_anchor=(0.5, -0.08),
        ncol=2 if len(classes) > 4 else 1,
        frameon=True,
        fontsize=8,
    )

    fig.tight_layout()
    fig.savefig(output_path, bbox_inches="tight")
    plt.close(fig)
