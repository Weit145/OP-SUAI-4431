POETRY ?= poetry
PYTHON := $(POETRY) run python
APP ?= app.py
PRODUCT ?=
RED ?=
NIR ?=
OUT ?= results
PROCESSED ?= data/processed

ifeq ($(strip $(PRODUCT)),)
PRODUCT_ARG =
else
PRODUCT_ARG = --product $(PRODUCT)
endif

.PHONY: help install run run-product run-files ui demo test check clean

help:
	@echo "Project commands:"
	@echo "  make install                         install Poetry dependencies"
	@echo "  make run                             run with automatic data search"
	@echo "  make run PRODUCT=L2A_...             run selected product from data"
	@echo "  make run-product PRODUCT=L2A_...     same as run, but PRODUCT is required"
	@echo "  make run-files RED=... NIR=...       run with explicit B04 and B08 files"
	@echo "  make ui                              start the Streamlit interface"
	@echo "  make demo                            create six multispectral demo bands"
	@echo "  make test                            run tests"
	@echo "  make check                           run Ruff checks"
	@echo "  make clean                           remove generated results"

install:
	$(POETRY) install

run:
	$(PYTHON) main.py $(PRODUCT_ARG) --out $(OUT) --processed $(PROCESSED)

run-product:
ifndef PRODUCT
	$(error Set PRODUCT: make run-product PRODUCT=L2A_T35VMF_A009487_20260630T095026)
endif
	$(PYTHON) main.py --product $(PRODUCT) --out $(OUT) --processed $(PROCESSED)

run-files:
ifndef RED
	$(error Set RED: make run-files RED=data/raw/B04.tif NIR=data/raw/B08.tif)
endif
ifndef NIR
	$(error Set NIR: make run-files RED=data/raw/B04.tif NIR=data/raw/B08.tif)
endif
	$(PYTHON) main.py --red $(RED) --nir $(NIR) --out $(OUT) --processed $(PROCESSED)

ui:
	$(POETRY) run streamlit run $(APP)

demo:
	$(PYTHON) download_data.py --demo

test:
	$(POETRY) run pytest

check:
	$(POETRY) run ruff check .
	$(POETRY) run ruff format --check .

clean:
	$(PYTHON) -c "from pathlib import Path; [p.unlink() for folder in ('results', 'data/processed') for p in Path(folder).glob('*') if p.is_file() and p.name != '.gitkeep']"
