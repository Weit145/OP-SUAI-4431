# Классификация растительности

Учебный Python-проект для обработки спутниковых снимков Sentinel-2. Программа загружает два спектральных канала, рассчитывает индекс NDVI, делит пиксели на классы растительности и сохраняет карты, таблицу статистики и короткий отчет.

## Что используется

Нужны два GeoTIFF-файла Sentinel-2:

- `B04.tif` - красный канал;
- `B08.tif` - ближний инфракрасный канал.

По умолчанию программа сначала ищет продукт Sentinel-2 L2A в папке `data/`, например `data/L2A_T35VMF_A009487_20260630T095026`, и берет каналы `IMG_DATA/R10m/*_B04_10m.jp2` и `IMG_DATA/R10m/*_B08_10m.jp2`. Если такого продукта нет, используются файлы `data/raw/B04.tif` и `data/raw/B08.tif`.

NDVI - это индекс, который показывает, насколько активно поверхность отражает ближний инфракрасный свет по сравнению с красным. У здоровой растительности NDVI обычно выше, у воды, застройки и голой почвы - ниже.

Формула:

```text
NDVI = (B08 - B04) / (B08 + B04)
```

## Установка

Требуется Python 3.10 или новее.

```powershell
cd "C:\Users\Weit\Documents\in_c\-\2 curs\2_half\UCH\vegetation-classification"
python -m venv .venv
.\.venv\Scripts\activate
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
```

Если виртуальное окружение не нужно, можно выполнить только:

```powershell
python -m pip install -r requirements.txt
```

## Как получить данные

### Вариант 1. Демонстрационные данные без интернета

Для проверки проекта можно создать маленькие учебные GeoTIFF-файлы:

```powershell
python download_data.py --demo
```

После этого появятся:

- `data/raw/B04.tif`;
- `data/raw/B08.tif`;
- `data/raw/demo_metadata.json`.

### Вариант 2. Автоматическое скачивание Sentinel-2

Скрипт умеет искать открытую сцену Sentinel-2 L2A в STAC-каталоге Microsoft Planetary Computer и сохранять фрагменты каналов B04/B08 по заданной области:

```powershell
python download_data.py
```

Можно указать свою область и даты:

```powershell
python download_data.py --bbox 30.20 59.96 30.38 60.05 --datetime 2024-06-01/2024-08-31 --cloud-cover 20
```

Если автоматическое скачивание не сработало из-за сети, API или авторизации, скачайте Sentinel-2 вручную через Copernicus Browser, Microsoft Planetary Computer или другой открытый источник. В проект нужно положить именно два файла:

```text
data/raw/B04.tif
data/raw/B08.tif
```

Файлы должны относиться к одной сцене и иметь одинаковый размер.

## Запуск

Стандартный запуск:

```powershell
python main.py
```

Запуск по названию папки продукта Sentinel-2 из `data`:

```powershell
python main.py L2A_T35VMF_A009487_20260630T095026
```

То же самое через именованный аргумент:

```powershell
python main.py --product L2A_T35VMF_A009487_20260630T095026
```

Запуск с явным указанием файлов:

```powershell
python main.py --red data/raw/B04.tif --nir data/raw/B08.tif --out results
```

Запуск с каналами из скачанного продукта Sentinel-2:

```powershell
python main.py --red "data/L2A_T35VMF_A009487_20260630T095026/IMG_DATA/R10m/T35VMF_20260630T095031_B04_10m.jp2" --nir "data/L2A_T35VMF_A009487_20260630T095026/IMG_DATA/R10m/T35VMF_20260630T095031_B08_10m.jp2"
```

## Запуск через Makefile

Если в системе установлен `make`, можно использовать короткие команды:

```powershell
make install
make run PRODUCT=L2A_T35VMF_A009487_20260630T095026
make check
```

Основные цели:

- `make install` - установить зависимости;
- `make run` - запустить программу с автоматическим поиском данных;
- `make run PRODUCT=L2A_...` - обработать конкретную папку продукта из `data`;
- `make run-files RED=... NIR=...` - запустить с явными путями к каналам;
- `make demo` - создать демонстрационные данные;
- `make clean` - удалить расчетные файлы из `results` и `data/processed`.

## Что появится в results

- `ndvi_map.png` - цветная карта NDVI;
- `classified_map.png` - карта классов растительности;
- `class_statistics.csv` - таблица с количеством пикселей и процентом каждого класса;
- `result_summary.txt` - короткий текстовый отчет.

В папке `data/processed/` дополнительно сохраняются расчетные GeoTIFF:

- `ndvi.tif`;
- `classified_vegetation.tif`.

## Классы растительности

| Класс | Диапазон NDVI | Значение |
|---:|---|---|
| 1 | `NDVI < 0.2` | нет растительности / вода / застройка |
| 2 | `0.2 <= NDVI < 0.4` | слабая растительность |
| 3 | `0.4 <= NDVI < 0.6` | средняя растительность |
| 4 | `NDVI >= 0.6` | густая растительность |

## Структура проекта

```text
vegetation-classification/
  README.md
  requirements.txt
  main.py
  download_data.py
  src/
    __init__.py
    ndvi.py
    classification.py
    visualization.py
    utils.py
  data/
    raw/
    processed/
  results/
    ndvi_map.png
    classified_map.png
    class_statistics.csv
    result_summary.txt
```

## Что делает каждая часть

- `download_data.py` скачивает каналы Sentinel-2 или создает демонстрационные данные.
- `main.py` запускает весь процесс обработки.
- `src/ndvi.py` считает NDVI.
- `src/classification.py` делит NDVI на четыре класса и считает статистику.
- `src/visualization.py` сохраняет PNG-карты.
- `src/utils.py` читает и записывает GeoTIFF, проверяет ошибки и формирует текстовый отчет.
