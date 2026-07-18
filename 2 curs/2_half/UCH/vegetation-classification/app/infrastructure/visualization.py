from __future__ import annotations

import io
import math
from typing import Sequence

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import BoundaryNorm, ListedColormap
from matplotlib.patches import Patch

from app.domain.classification import MapClass


def _preview(array: np.ndarray, max_side: int = 2_000) -> np.ndarray:
    step = max(1, math.ceil(max(array.shape) / max_side))
    return array[::step, ::step]


def render_index_map(
    index: np.ndarray,
    *,
    title: str,
    label: str,
    cmap: str = "RdYlGn",
) -> bytes:
    fig, ax = plt.subplots(figsize=(9, 7), dpi=150)
    image = ax.imshow(_preview(index), cmap=cmap, vmin=-1, vmax=1)
    ax.set_title(title)
    ax.set_axis_off()
    colorbar = fig.colorbar(image, ax=ax, fraction=0.046, pad=0.04)
    colorbar.set_label(label)
    fig.tight_layout()
    buffer = io.BytesIO()
    fig.savefig(buffer, format="png", bbox_inches="tight")
    plt.close(fig)
    return buffer.getvalue()


def render_classified_map(
    classified: np.ndarray,
    *,
    classes: Sequence[MapClass],
    title: str,
) -> bytes:
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
    buffer = io.BytesIO()
    fig.savefig(buffer, format="png", bbox_inches="tight")
    plt.close(fig)
    return buffer.getvalue()
