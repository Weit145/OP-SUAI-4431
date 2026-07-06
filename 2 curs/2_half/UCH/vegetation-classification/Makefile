PYTHON ?= python
PRODUCT ?=
RED ?=
NIR ?=
OUT ?= results
PROCESSED ?= data/processed

ifeq ($(strip $(PRODUCT)),)
PRODUCT_ARG =
else
PRODUCT_ARG = $(PRODUCT)
endif

.PHONY: help install venv install-venv run run-product run-files demo check clean

help:
	@echo "Project commands:"
	@echo "  make install                         install dependencies"
	@echo "  make venv                            create .venv"
	@echo "  make install-venv                    install dependencies into .venv"
	@echo "  make run                             run with automatic data search"
	@echo "  make run PRODUCT=L2A_...             run selected product from data"
	@echo "  make run-product PRODUCT=L2A_...     same as run, but PRODUCT is required"
	@echo "  make run-files RED=... NIR=...       run with explicit B04 and B08 files"
	@echo "  make demo                            create demo B04.tif and B08.tif"
	@echo "  make check                           check Python syntax"
	@echo "  make clean                           remove generated results"

install:
	$(PYTHON) -m pip install -r requirements.txt

venv:
	$(PYTHON) -m venv .venv

install-venv: venv
	.venv/Scripts/python.exe -m pip install --upgrade pip
	.venv/Scripts/python.exe -m pip install -r requirements.txt

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

demo:
	$(PYTHON) download_data.py --demo

check:
	$(PYTHON) -m compileall main.py download_data.py src

clean:
	$(PYTHON) -c "from pathlib import Path; [p.unlink() for folder in ('results', 'data/processed') for p in Path(folder).glob('*') if p.is_file() and p.name != '.gitkeep']"
