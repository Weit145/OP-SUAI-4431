#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from datetime import datetime
from html import escape
from pathlib import Path
from urllib.parse import parse_qs


def read_form_data(method):
    """Получение данных формы для POST и GET без использования устаревшего модуля cgi."""
    if method == "POST":
        length = int(os.environ.get("CONTENT_LENGTH", "0") or 0)
        raw_data = sys.stdin.buffer.read(length).decode("utf-8", errors="replace")
    else:
        raw_data = os.environ.get("QUERY_STRING", "")
    return parse_qs(raw_data, keep_blank_values=True)


def get_value(form, field):
    values = form.get(field, [])
    if not values:
        return "(не заполнено)"
    clean_values = [value.strip() for value in values if value.strip()]
    if not clean_values:
        return "(не заполнено)"
    return ", ".join(clean_values)


request_method = os.environ.get("REQUEST_METHOD", "GET").upper()
form = read_form_data(request_method)

fields = [
    ("Фамилия", "surname"),
    ("Имя", "name"),
    ("Отчество", "patronymic"),
    ("Цель посещения", "goal"),
    ("Интересующие разделы", "topics"),
    ("Уровень знакомства", "experience"),
    ("Тема сайта", "site_theme"),
    ("Вариант задания", "variant"),
]

data = {key: get_value(form, key) for _, key in fields}

# Расширенное задание: накопительная запись значений в текстовый файл.
# Названия полей не сохраняются, только значения анкеты.
result_file = Path(__file__).resolve().parent / "anketa_results.txt"
line_values = [datetime.now().strftime("%Y-%m-%d %H:%M:%S")]
line_values.extend(data[key] for _, key in fields)
with result_file.open("a", encoding="utf-8") as file:
    file.write("; ".join(line_values) + "\n")

print("Content-type: text/html; charset=utf-8")
print()
print(f"""<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="utf-8">
    <title>Ответ сервера</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <header class="gradient-bg" style="padding:20px; text-align:center;">
            <h1>Статистика и аналитика данных</h1>
            <h2>Ответ сервера</h2>
        </header>

        <nav style="background:#bbdefb; padding:10px; text-align:center;">
            <a href="/first.html">Главная</a> |
            <a href="/three.html">Методы и инструменты</a> |
            <a href="/sekond.html">Источники</a> |
            <a href="/lab5.html">Анкета</a> |
            <a href="/lab6.php">PHP и БД</a> |
            <a href="/lab7.html">AJAX</a> |
            <a href="/lab8.html">XML</a>
        </nav>

        <main class="lab5-main">
            <section class="response-card">
                <h1>Анкета пользователя получена</h1>
                <p>Введённые данные обработаны серверным сценарием Python методом {escape(request_method)}.</p>
                <dl class="response-list">""")

for label, key in fields:
    print(f"<dt>{escape(label)}</dt>")
    print(f"<dd>{escape(data[key])}</dd>")

print("""                </dl>
                <p>Результат также добавлен в текстовый файл <code>cgi-bin/anketa_results.txt</code>.</p>
                <a class="back-link" href="/lab5.html">Вернуться к анкете</a>
            </section>
        </main>

        <footer>
            <time datetime="2026">2026</time> Кочнев Д. О.
        </footer>
    </div>
</body>
</html>""")
