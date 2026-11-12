-- Вариант 13. Калькулятор бюджета семьи/группы. Запросы а–в.
-- Выполнять после create_table.sql и create_test_value.sql.
-- Без вложенных запросов и агрегатных функций.

-- а) Статьи расходов категорий, название которых содержит «мед»,
-- но не заканчивается на него.
-- LIKE учитывает регистр: в тестовых данных используется «Мед».
-- % — любое число символов, _ — ровно один символ.
-- Поэтому после «Мед» должен быть хотя бы один символ.
SELECT c.name AS category_name, item.name AS expense_item
FROM expense_categories AS c
JOIN expense_items AS item
    ON item.id_expense_category = c.id_expense_category
WHERE c.name LIKE '%Мед_%'
ORDER BY c.name, item.name;
-- Ожидается: «Медицинские услуги» — «Приём врача», «Стоматология».
-- Категория «Мед» и её статья «Страховка» не попадают в результат.

-- б) Пользователь, у которого было несколько доходов разных категорий
-- в одном месяце одного года.
-- Два экземпляра incomes находят пару доходов одного пользователя.
-- Два экземпляра income_items позволяют сравнить их категории.
-- DISTINCT убирает повторы пользователя при наличии нескольких пар.
SELECT DISTINCT u.id_user, u.nickname
FROM users AS u
JOIN incomes AS i1 ON i1.id_user = u.id_user
JOIN incomes AS i2
    ON i2.id_user = u.id_user
    AND i1.id_income <> i2.id_income
    AND EXTRACT(YEAR FROM i1.date_income) = EXTRACT(YEAR FROM i2.date_income)
    AND EXTRACT(MONTH FROM i1.date_income) = EXTRACT(MONTH FROM i2.date_income)
JOIN income_items AS item1 ON item1.id_income_item = i1.id_income_item
JOIN income_items AS item2 ON item2.id_income_item = i2.id_income_item
WHERE item1.id_income_category <> item2.id_income_category
ORDER BY u.nickname;
-- Ожидается: Иван (зарплата и продажа в январе 2026 года).

-- в) Статья доходов без доходов по ней.
-- LEFT JOIN сохраняет статьи, даже если соответствующих доходов нет.
SELECT item.name AS income_item, c.name AS category_name
FROM income_items AS item
JOIN income_categories AS c
    ON c.id_income_category = item.id_income_category
LEFT JOIN incomes AS i ON i.id_income_item = item.id_income_item
WHERE i.id_income IS NULL
ORDER BY item.name;
-- Ожидается: «Премия», категория «Зарплата».

-- в) Тот же результат с RIGHT JOIN.
-- Статьи находятся справа: они сохраняются даже без доходов.
SELECT item.name AS income_item, c.name AS category_name
FROM incomes AS i
RIGHT JOIN income_items AS item ON i.id_income_item = item.id_income_item
JOIN income_categories AS c
    ON c.id_income_category = item.id_income_category
WHERE i.id_income IS NULL
ORDER BY item.name;
-- Ожидается: «Премия», категория «Зарплата».
