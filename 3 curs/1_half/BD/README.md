# Учебная база данных

Запуск из этой папки:

```bash
docker compose up -d --wait
```

При первом запуске PostgreSQL автоматически выполняет `create_table.sql`,
затем `create_test_value.sql`: создаёт семь таблиц и загружает тестовые данные.
Повторные запуски сохраняют данные в томе `db_data` и не выполняют эти SQL-файлы заново.

Подключение по умолчанию: `localhost:5432`, база `postgres`,
пользователь `postgres`, пароль `postgres`.
Эти значения можно переопределить переменными `POSTGRES_DB`,
`POSTGRES_USER` и `POSTGRES_PASSWORD` перед первым запуском.

Открыть SQL-консоль:

```bash
docker compose exec db sh -c 'psql -U "$POSTGRES_USER" -d "$POSTGRES_DB"'
```

Остановить БД с сохранением данных:

```bash
docker compose down
```
