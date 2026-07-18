const modeButtons = document.querySelectorAll("[data-mode]");
const modePanels = document.querySelectorAll("[data-panel]");
const form = document.querySelector("#analysis-form");
const submitButton = form.querySelector(".submit-button");
const statusBox = document.querySelector("#status-box");
const errorBox = document.querySelector("#error-box");
const resultsSection = document.querySelector("#results");

let activeMode = "ndvi";

const previewLabels = {
  classification: "Карта классов",
  ndvi: "Индекс NDVI",
  water: "Водный индекс MNDWI",
  built_up: "Индекс застройки NDBI",
};

function selectMode(mode) {
  activeMode = mode;
  modeButtons.forEach((button) => {
    const isActive = button.dataset.mode === mode;
    button.classList.toggle("active", isActive);
    button.setAttribute("aria-selected", String(isActive));
  });
  modePanels.forEach((panel) => {
    panel.classList.toggle("hidden", panel.dataset.panel !== mode);
  });
  hideError();
}

modeButtons.forEach((button) => {
  button.addEventListener("click", () => selectMode(button.dataset.mode));
});

document.querySelectorAll("[data-file-drop]").forEach((drop) => {
  const input = drop.querySelector("input[type='file']");
  const filename = drop.querySelector("[data-file-name]");
  const fallback = filename.textContent;

  function updateFile() {
    const file = input.files[0];
    filename.textContent = file ? file.name : fallback;
    drop.classList.toggle("has-file", Boolean(file));
  }

  input.addEventListener("change", updateFile);
  ["dragenter", "dragover"].forEach((eventName) => {
    drop.addEventListener(eventName, (event) => {
      event.preventDefault();
      drop.classList.add("dragover");
    });
  });
  ["dragleave", "drop"].forEach((eventName) => {
    drop.addEventListener(eventName, (event) => {
      event.preventDefault();
      drop.classList.remove("dragover");
    });
  });
  drop.addEventListener("drop", (event) => {
    if (event.dataTransfer.files.length) {
      input.files = event.dataTransfer.files;
      updateFile();
    }
  });
});

function hideError() {
  errorBox.textContent = "";
  errorBox.classList.add("hidden");
}

function showError(message) {
  errorBox.textContent = message;
  errorBox.classList.remove("hidden");
}

function setLoading(isLoading) {
  submitButton.disabled = isLoading;
  submitButton.querySelector("span").textContent = isLoading
    ? "Обработка…"
    : "Запустить анализ";
  statusBox.classList.toggle("hidden", !isLoading);
}

function requireFiles() {
  if (activeMode === "ndvi") {
    const red = document.querySelector("#red-file").files[0];
    const nir = document.querySelector("#nir-file").files[0];
    if (!red || !nir) {
      throw new Error("Добавьте оба канала: B04 и B08.");
    }
    return { red, nir };
  }
  const archive = document.querySelector("#archive-file").files[0];
  if (!archive) {
    throw new Error("Добавьте ZIP-архив сцены Sentinel-2.");
  }
  return { archive };
}

async function readError(response) {
  try {
    const payload = await response.json();
    if (typeof payload.detail === "string") return payload.detail;
    if (Array.isArray(payload.detail)) {
      return payload.detail.map((item) => item.msg).join("; ");
    }
  } catch (_) {
    // Nginx can return a plain-text error for an oversized request.
  }
  return response.status === 413
    ? "Файл превышает допустимый размер загрузки."
    : `Сервер вернул ошибку ${response.status}.`;
}

function formatPixels(value) {
  return new Intl.NumberFormat("ru-RU").format(value);
}

function renderPreviews(previews) {
  const grid = document.querySelector("#preview-grid");
  grid.replaceChildren();
  Object.entries(previews).forEach(([key, url], index) => {
    const figure = document.createElement("figure");
    figure.className = "preview-card";

    const image = document.createElement("img");
    image.src = url;
    image.alt = previewLabels[key] || key;
    image.loading = index === 0 ? "eager" : "lazy";

    const caption = document.createElement("figcaption");
    const title = document.createElement("strong");
    title.textContent = previewLabels[key] || key;
    const format = document.createElement("span");
    format.textContent = "PNG preview";
    caption.append(title, format);
    figure.append(image, caption);
    grid.append(figure);
  });
}

function renderStatistics(statistics) {
  const body = document.querySelector("#statistics-body");
  body.replaceChildren();
  statistics.forEach((item) => {
    const row = document.createElement("tr");

    const nameCell = document.createElement("td");
    const name = document.createElement("span");
    name.className = "class-name";
    const dot = document.createElement("i");
    dot.className = "color-dot";
    dot.style.backgroundColor = item.color;
    name.append(dot, document.createTextNode(item.name));
    nameCell.append(name);

    const conditionCell = document.createElement("td");
    conditionCell.textContent = item.condition;

    const pixelsCell = document.createElement("td");
    pixelsCell.textContent = formatPixels(item.pixel_count);

    const percentCell = document.createElement("td");
    percentCell.className = "percent-cell";
    const percentValue = document.createElement("div");
    percentValue.className = "percent-value";
    percentValue.textContent = `${item.percent.toFixed(2)}%`;
    const track = document.createElement("div");
    track.className = "progress-track";
    const fill = document.createElement("span");
    fill.style.width = `${Math.max(0, Math.min(item.percent, 100))}%`;
    fill.style.backgroundColor = item.color;
    track.append(fill);
    percentCell.append(percentValue, track);

    row.append(nameCell, conditionCell, pixelsCell, percentCell);
    body.append(row);
  });
}

function renderResult(result) {
  document.querySelector("#result-mode").textContent =
    result.mode === "ndvi" ? "NDVI" : "Полный спектр";
  document.querySelector("#result-pixels").textContent = formatPixels(
    result.valid_pixel_count,
  );
  renderPreviews(result.previews);
  renderStatistics(result.statistics);
  resultsSection.classList.remove("hidden");
  resultsSection.scrollIntoView({ behavior: "smooth", block: "start" });
}

form.addEventListener("submit", async (event) => {
  event.preventDefault();
  hideError();

  let files;
  try {
    files = requireFiles();
  } catch (error) {
    showError(error.message);
    return;
  }

  const data = new FormData();
  let endpoint;
  if (activeMode === "ndvi") {
    endpoint = "/api/v1/analyses/ndvi";
    data.append("red", files.red);
    data.append("nir", files.nir);
  } else {
    endpoint = "/api/v1/analyses/multispectral";
    data.append("archive", files.archive);
  }

  setLoading(true);
  try {
    const response = await fetch(endpoint, { method: "POST", body: data });
    if (!response.ok) throw new Error(await readError(response));
    renderResult(await response.json());
  } catch (error) {
    showError(error.message || "Не удалось связаться с сервером.");
  } finally {
    setLoading(false);
  }
});
