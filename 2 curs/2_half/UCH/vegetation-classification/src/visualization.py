from __future__ import annotations

from pathlib import Path

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import BoundaryNorm, ListedColormap
from matplotlib.patches import Patch

from src.classification import VEGETATION_CLASSES


def save_ndvi_map(ndvi: np.ndarray, output_path: Path) -> None:
    """Сохраняет цветную PNG-карту рассчитанного индекса NDVI."""
    output_path.parent.mkdir(parents=True, exist_ok=True)

    fig, ax = plt.subplots(figsize=(9, 7), dpi=150)
    image = ax.imshow(ndvi, cmap="RdYlGn", vmin=-1, vmax=1)
    ax.set_title("Карта NDVI")
    ax.set_axis_off()

    colorbar = fig.colorbar(image, ax=ax, fraction=0.046, pad=0.04)
    colorbar.set_label("NDVI")

    fig.tight_layout()
    fig.savefig(output_path, bbox_inches="tight")
    plt.close(fig)


def save_classified_map(classified: np.ndarray, output_path: Path) -> None:
    """Сохраняет PNG-карту классов растительности с легендой."""
    output_path.parent.mkdir(parents=True, exist_ok=True)

    colors = [item.color for item in VEGETATION_CLASSES]
    cmap = ListedColormap(colors)
    norm = BoundaryNorm([0.5, 1.5, 2.5, 3.5, 4.5], cmap.N)
    masked = np.ma.masked_where(classified == 0, classified)

    fig, ax = plt.subplots(figsize=(9, 7), dpi=150)
    ax.imshow(masked, cmap=cmap, norm=norm)
    ax.set_title("Классификация растительности по NDVI")
    ax.set_axis_off()

    legend_items = [
        Patch(facecolor=item.color, edgecolor="black", label=f"{item.class_id}. {item.name}")
        for item in VEGETATION_CLASSES
    ]
    ax.legend(
        handles=legend_items,
        loc="lower center",
        bbox_to_anchor=(0.5, -0.10),
        ncol=1,
        frameon=True,
        fontsize=8,
    )

    fig.tight_layout()
    fig.savefig(output_path, bbox_inches="tight")
    plt.close(fig)
