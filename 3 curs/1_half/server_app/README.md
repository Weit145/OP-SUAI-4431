# Лабораторная работа №2

Вариант 14: «Сдача недвижимости в аренду».

Приложение предоставляет REST/JSON API для CRUD-операций над объектами недвижимости. Данные хранятся во встроенной H2, схема создаётся Liquibase.

## Запуск

```bash
mvn spring-boot:run
```

После запуска:

- Swagger UI: <http://localhost:8080/swagger-ui.html>
- OpenAPI JSON: <http://localhost:8080/api-docs>
- REST API: <http://localhost:8080/api/properties>

Исходное OpenAPI v3-описание для отчёта находится в `openapi.yaml`.

## Методы

| HTTP | URI | Код успеха | Назначение |
|---|---|---:|---|
| GET | `/api/properties` | 200 | получить все объекты |
| GET | `/api/properties/{id}` | 200 | получить объект |
| POST | `/api/properties` | 201 | создать объект |
| PUT | `/api/properties/{id}` | 200 | обновить объект |
| DELETE | `/api/properties/{id}` | 204 | удалить объект |

Пример JSON для POST/PUT:

```json
{
  "address": "Санкт-Петербург, Литейный проспект, 10",
  "type": "APARTMENT",
  "area": 42.5,
  "rooms": 2,
  "monthlyRent": 55000,
  "available": true
}
```

Допустимые типы: `APARTMENT`, `HOUSE`, `ROOM`, `COMMERCIAL`.
