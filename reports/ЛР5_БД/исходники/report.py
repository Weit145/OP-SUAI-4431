from pathlib import Path
from copy import deepcopy
import re
import shutil

from docx import Document
from docx.shared import Cm, Pt
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn

BASE = Path(__file__).resolve().parent
OUT = BASE.parent
ROOT = OUT.parent.parent
TEMPLATE = ROOT / 'reports/ЛР4_БД/ЛР4_Кочнев_4431_исправленный.docx'
doc = Document(TEMPLATE)
body = doc._element.body
original = list(body)
model = deepcopy(original[30])
# Сохраняем титульный лист, стили, колонтитулы и изображение модели образца.
for element in original[12:]:
    if element.tag != qn('w:sectPr'):
        body.remove(element)
for paragraph in body.iter(qn('w:p')):
    texts = list(paragraph.iter(qn('w:t')))
    text = ''.join(t.text or '' for t in texts)
    replacement = None
    if 'ОТЧЕТ О ЛАБОРАТОРНОЙ РАБОТЕ' in text:
        replacement = 'ОТЧЕТ О ЛАБОРАТОРНОЙ РАБОТЕ №5'
    if 'ЗАПОЛНЕНИЕ ТАБЛИЦ И МОДИФИКАЦИЯ ДАННЫХ' in text:
        replacement = 'РАЗРАБОТКА SQL ЗАПРОСОВ: ВИДЫ СОЕДИНЕНИЙ И ШАБЛОНЫ'
    if replacement is not None:
        texts[0].text = replacement
        for t in texts[1:]:
            t.text = ''

def make_style(name, size, font='Times New Roman', bold=False):
    style = doc.styles.add_style(name, 1)
    style.font.name = font
    style.font.size = Pt(size)
    style.font.bold = bold
    pf = style.paragraph_format
    pf.space_before = Pt(0)
    pf.space_after = Pt(6)
    pf.line_spacing = 1.5
    pf.first_line_indent = Cm(1.25)
    return style

make_style('ReportBody5', 14)
heading = make_style('ReportHeading5', 14, bold=True)
heading.paragraph_format.first_line_indent = Cm(0)
heading.paragraph_format.keep_with_next = True
heading.paragraph_format.space_after = Pt(12)
code_style = make_style('ReportCode5', 9, 'DejaVu Sans Mono')
code_style.paragraph_format.first_line_indent = Cm(0)
code_style.paragraph_format.line_spacing = 1
code_style.paragraph_format.space_after = Pt(0)
caption_style = make_style('ReportCaption5', 12)
caption_style.paragraph_format.first_line_indent = Cm(0)
caption_style.paragraph_format.line_spacing = 1

def p(text):
    paragraph = doc.add_paragraph(text, 'ReportBody5')
    paragraph.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY
    return paragraph

def h(text, page=False):
    paragraph = doc.add_paragraph(text, 'ReportHeading5')
    paragraph.paragraph_format.page_break_before = page
    return paragraph

def caption(text):
    paragraph = doc.add_paragraph(text, 'ReportCaption5')
    paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER

def picture(path, width):
    paragraph = doc.add_paragraph()
    paragraph.paragraph_format.space_after = Pt(5)
    paragraph.paragraph_format.first_line_indent = Cm(0)
    paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER
    paragraph.add_run().add_picture(str(path), width=Cm(width))

def table(headers, rows):
    t = doc.add_table(rows=1, cols=len(headers))
    t.style = 'Table Grid'
    for cell, value in zip(t.rows[0].cells, headers):
        cell.text = value
    for values in rows:
        for cell, value in zip(t.add_row().cells, values):
            cell.text = value
    for row_index, row in enumerate(t.rows):
        trpr = row._tr.get_or_add_trPr()
        trpr.append(OxmlElement('w:cantSplit'))
        if row_index == 0:
            trpr.append(OxmlElement('w:tblHeader'))
        for cell in row.cells:
            for paragraph in cell.paragraphs:
                paragraph.paragraph_format.line_spacing = 1
                paragraph.paragraph_format.space_after = Pt(5)
                for run in paragraph.runs:
                    run.font.name = 'Times New Roman'
                    run.font.size = Pt(12)
                    run.bold = row_index == 0
    return t

sql_path = ROOT / '3 curs/1_half/BD/select_queries.sql'
sql = re.sub(r'--[^\n]*', '', sql_path.read_text())
queries = [s.strip() + ';' for s in sql.split(';') if s.strip()]

def code(index):
    # Переносы длинных строк меняют только оформление SQL.
    text = queries[index]
    text = text.replace(' = EXTRACT', '\n        = EXTRACT')
    text = text.replace('JOIN income_items AS item1 ON', 'JOIN income_items AS item1\n    ON')
    text = text.replace('JOIN income_items AS item2 ON', 'JOIN income_items AS item2\n    ON')
    for line in text.splitlines():
        paragraph = doc.add_paragraph(line, 'ReportCode5')
        paragraph.paragraph_format.keep_with_next = True
    doc.paragraphs[-1].paragraph_format.keep_with_next = False

assets = BASE / 'изображения'
assets.mkdir(parents=True, exist_ok=True)
screens = Path('/home/weit/Изображения/Снимки экрана')
for suffix, name in [('12', 'result_a.png'), ('30', 'result_b.png'), ('43', 'result_c.png')]:
    source = screens / f'Снимок экрана от 2026-11-08 21-07-{suffix}.png'
    shutil.copy2(source, assets / name)

h('1. Цель работы', page=True)
p('Получение навыков создания запросов без подзапросов, используя различные виды соединений.')
h('2. Задание')
p('1) Реализовать запросы а) .. в), указанные в варианте задания.')
for text in [
    '– Запрос на поиск по шаблону (поиск подстроки)( выполнить с единственным оператором like без использования функций left, RIGHT, length)',
    '– Запрос на использование одной таблицы несколько раз (псевдонимы).',
    '– Запрос на использование внешних соединений.',
]:
    p(text)
p('Все запросы НЕ должны содержать вложенных запросов или агрегатных функций (используйте псевдонимы и различные виды соединений вместо них).')
p('Цель и задание приведены по методическим указаниям «Современные реляционные базы данных», составители Н. В. Путилова, М. В. Величко, Санкт-Петербург, 2025, с. 60–61. Текст варианта приведён на с. 97–98.')

h('3. Вариант задания', page=True)
p('13. калькулятор бюджета семьи/группы: категория дохода (продажа, зарплата), категория расхода (еда, счета за КУ, здоровье …), статьи дохода и расхода, дата расхода/дохода. Категория- более общее понятие чем статья. Например категория- еда, а статьи в ней мясо, рыба, вкусное к чаю, а конкретный расход «печенье «курабье» к чаю 11.09»')
for text in [
    'а. статьи расходов всех категорий название, которых (категорий) содержат слово «мед», но не заканчиваются на него',
    'б. пользователь, у которого было несколько доходов разных категорий в одном месяце',
    'в. Статья доходов без доходов по ней',
    'г. месяц, в котором был максимальный единоразовый(не совокупность, а одно дело) расход в текущем году',
    'д. категории, по которой не было доходов в мае текущего года, но были в январе(любом)',
    'е. категория(категории) расхода, по которой были траты в январе прошлого года у всех пользователей',
    'ж. месяц и год, в котором были расходы количества статей меньше среднего',
]:
    p(text)
p('В лабораторной работе № 5 выполняются пункты а–в варианта 13.')

h('4. Физическая модель базы данных', page=True)
p('База данных реализована в PostgreSQL и содержит семь таблиц: users, income_categories, income_items, incomes, expense_categories, expense_items и expenses. Физическая модель перенесена из предыдущей лабораторной работы.')
doc._element.body.insert(len(doc._element.body) - 1, model)
caption('Рисунок 1 – Физическая модель базы данных')
p('Первичные ключи имеют тип UUID и создаются с помощью gen_random_uuid(). Названия хранятся в VARCHAR(256), суммы — в DECIMAL(12, 2), даты — в DATE. Внешние ключи связывают статьи с категориями, а доходы и расходы — с пользователями и статьями. Для внешних ключей заданы ON DELETE CASCADE и ON UPDATE RESTRICT.')

h('5. Тестовые данные для запросов а–в', page=True)
p('Использован исходный набор из лабораторной работы № 4, загруженный скриптом create_test_value.sql. Ниже представлены элементы таблицы тестовых данных, необходимые для проверки запросов а–в, до выполнения операций изменения и удаления.')
caption('Таблица 1 – Соответствующие и несоответствующие примеры')
table(['Запрос', 'Данные, удовлетворяющие условию', 'Данные, не удовлетворяющие условию'], [
    ('а) Поиск статей по названию категории', '«Приём врача» и «Стоматология»: категория «Медицинские услуги» содержит «Мед» и не заканчивается на него.', '«Страховка»: категория «Мед» заканчивается на искомую подстроку. «Мясо», «Рыба», «Сладости к чаю»: категория «Еда» не содержит её.'),
    ('б) Доходы разных категорий в одном месяце', 'Иван: 08.01.2026 — «Оклад», 85 000 руб., категория «Зарплата»; 18.01.2026 — «Продажа техники», 14 000 руб., категория «Продажа».', 'Мария: доходы только категории «Подработка» — 11.01.2026 и 15.05.2026. Алексей: доходы только категории «Зарплата» — 24.01.2026 и 20.05.2026.'),
    ('в) Статья без доходов', '«Премия» (категория «Зарплата»): в incomes нет записей по этой статье.', '«Оклад»: 4 записи; «Продажа техники»: 1 запись; «Фриланс»: 2 записи в incomes.'),
])
p('В таблице показаны названия связанных записей вместо UUID для удобства чтения. SQL-запросы связывают записи по первичным и внешним ключам.')

h('6. Выполнение SQL-запросов', page=True)
h('6.1. Поиск по шаблону — запрос а)')
code(0)
p('Шаблон %Мед_% содержит один оператор LIKE. Символ % обозначает любое число символов, а _ требует хотя бы один символ после «Мед». В исходных данных используется заглавная «М», поскольку LIKE в PostgreSQL учитывает регистр. Дополнительные функции обработки строк не используются.')
picture(assets / 'result_a.png', 12)
caption('Рисунок 2 – Результат запроса а)')

h('6.2. Повторное использование таблиц — запрос б)', page=True)
code(1)
p('Таблица incomes используется дважды под псевдонимами i1 и i2, а income_items — под псевдонимами item1 и item2. Сопоставляются разные доходы одного пользователя с различными категориями. EXTRACT извлекает год и месяц: сравнение обоих значений исключает совпадение месяцев разных лет. DISTINCT удаляет повторения пользователя; агрегатные функции не используются.')
picture(assets / 'result_b.png', 15)
caption('Рисунок 3 – Результат запроса б)')
p('Возвращён Иван: в январе 2026 года у него были доходы категорий «Зарплата» и «Продажа».')

h('6.3. Внешнее соединение — запрос в)', page=True)
code(2)
p('LEFT JOIN сохраняет статьи доходов, даже если записей о доходах по ним нет. Для таких строк столбцы incomes равны NULL. Условие i.id_income IS NULL оставляет только статьи без доходов.')
picture(assets / 'result_c.png', 14)
caption('Рисунок 4 – Результат запроса в)')
p('Возвращена статья «Премия» категории «Зарплата». Эквивалентный запрос с RIGHT JOIN:')
code(3)
p('При RIGHT JOIN сохраняется таблица income_items, расположенная справа. Оба запроса возвращают один и тот же набор данных.')

h('7. Выводы', page=True)
p('В ходе лабораторной работы реализованы три запроса варианта 13 без вложенных запросов и агрегатных функций. Поиск статей расходов выполнен одним оператором LIKE с шаблоном %Мед_%; сочетание % и _ позволяет потребовать наличие символа после искомой подстроки. При выборе шаблона необходимо учитывать регистр исходных данных.')
p('Для поиска пользователей с доходами разных категорий одна таблица доходов и одна таблица статей использованы несколько раз с разными псевдонимами. Сравнение года и месяца позволяет правильно ограничить пары доходов одним календарным месяцем, а DISTINCT устраняет повторения в результате.')
p('Для поиска статей без доходов использовано внешнее соединение и проверка первичного ключа присоединяемой таблицы на NULL. LEFT JOIN и RIGHT JOIN дают одинаковый результат при соответствующей перестановке таблиц. Внутреннее соединение для такой задачи непригодно, поскольку исключает строки без соответствия.')
p('Полученные результаты соответствуют тестовым примерам: для запроса а) найдены статьи «Приём врача» и «Стоматология», для б) — пользователь Иван, для в) — статья «Премия». Все запросы выполняют только выборку и не изменяют данные.')

destination = OUT / 'ЛР5_Кочнев_4431.docx'
doc.save(destination)
shutil.copy2(sql_path, BASE / 'select_queries.sql')
print(destination)
