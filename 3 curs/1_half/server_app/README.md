# Лабораторная работа №4

Вариант 14: «Сдача недвижимости в аренду».

Приложение развивает REST API второй лабораторной: добавлены AngularJS и Thymeleaf, вход через Spring Security, HTTPS, CSRF, Logout и журнал изменений. Решение адаптировано из [официального lab4example](https://github.com/wildpierre/trsissamples/tree/master/lab4example) под аренду недвижимости.

## Запуск

```bash
make
```

`make` запускает тесты и HTTPS-сервер. Для запуска без тестов используйте `make run`. Maven создаёт учебный самоподписанный сертификат в `target/keystore.p12`. Браузер при первом открытии попросит подтвердить его.

- Страница объектов: <https://localhost:8443/>
- Вход: <https://localhost:8443/login>
- Swagger UI: <https://localhost:8443/swagger-ui.html>
- OpenAPI JSON: <https://localhost:8443/api-docs>
- REST API: <https://localhost:8443/api/properties>

Учебный логин — `guest`, пароль — `hello` (как в примере преподавателя). В базе хранится BCrypt-хэш. H2 находится в памяти: изменения и аудит сбрасываются при остановке сервера. Сертификат и учётные данные предназначены для демонстрации лабораторной.

Исходное OpenAPI v3-описание для отчёта находится в `openapi.yaml`. Дополнительные команды: `make test`, `make build`, `make clean`, `make help`.

## Методы

| HTTP | URI | Код успеха | Доступ |
|---|---|---:|---|
| GET | `/api/properties` | 200 | всем |
| GET | `/api/properties/{id}` | 200 | всем |
| POST | `/api/properties` | 201 | после входа + CSRF |
| PUT | `/api/properties/{id}` | 200 | после входа + CSRF |
| DELETE | `/api/properties/{id}` | 204 | после входа + CSRF |
| GET | `/api/audit` | 200 | после входа |

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

На защите можно показать: анонимный просмотр, вход, изменение через AJAX, HTTP 403 без CSRF, журнал `CREATE/UPDATE/DELETE` и кнопку выхода.
