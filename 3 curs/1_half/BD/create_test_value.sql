-- Запускать после create_table.sql на пустых таблицах.
BEGIN;

INSERT INTO users (nickname) VALUES
    ('Иван'), ('Мария'), ('Алексей');

INSERT INTO income_categories (name) VALUES
    ('Зарплата'), ('Продажа'), ('Подработка');

INSERT INTO income_items (name, id_income_category) VALUES
    ('Оклад', (SELECT id_income_category FROM income_categories WHERE name = 'Зарплата')),
    ('Премия', (SELECT id_income_category FROM income_categories WHERE name = 'Зарплата')),
    ('Продажа техники', (SELECT id_income_category FROM income_categories WHERE name = 'Продажа')),
    ('Фриланс', (SELECT id_income_category FROM income_categories WHERE name = 'Подработка'));

INSERT INTO incomes (amount, date_income, description_income, id_user, id_income_item) VALUES
    (85000, DATE '2026-01-08', 'Зарплата за декабрь',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_income_item FROM income_items WHERE name = 'Оклад')),
    (14000, DATE '2026-01-18', 'Продажа видеокарты',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_income_item FROM income_items WHERE name = 'Продажа техники')),
    (22000, DATE '2026-01-11', 'Разработка сайта',
        (SELECT id_user FROM users WHERE nickname = 'Мария'),
        (SELECT id_income_item FROM income_items WHERE name = 'Фриланс')),
    (60000, DATE '2026-01-24', 'Зарплата',
        (SELECT id_user FROM users WHERE nickname = 'Алексей'),
        (SELECT id_income_item FROM income_items WHERE name = 'Оклад')),
    (85000, DATE '2026-05-06', 'Зарплата',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_income_item FROM income_items WHERE name = 'Оклад')),
    (35000, DATE '2026-05-15', 'Разработка приложения',
        (SELECT id_user FROM users WHERE nickname = 'Мария'),
        (SELECT id_income_item FROM income_items WHERE name = 'Фриланс')),
    (62000, DATE '2026-05-20', 'Зарплата',
        (SELECT id_user FROM users WHERE nickname = 'Алексей'),
        (SELECT id_income_item FROM income_items WHERE name = 'Оклад'));

INSERT INTO expense_categories (name) VALUES
    ('Еда'), ('Коммунальные услуги'), ('Медицинские услуги'), ('Мед');

INSERT INTO expense_items (name, id_expense_category) VALUES
    ('Мясо', (SELECT id_expense_category FROM expense_categories WHERE name = 'Еда')),
    ('Рыба', (SELECT id_expense_category FROM expense_categories WHERE name = 'Еда')),
    ('Сладости к чаю', (SELECT id_expense_category FROM expense_categories WHERE name = 'Еда')),
    ('Электроэнергия', (SELECT id_expense_category FROM expense_categories WHERE name = 'Коммунальные услуги')),
    ('Водоснабжение', (SELECT id_expense_category FROM expense_categories WHERE name = 'Коммунальные услуги')),
    ('Приём врача', (SELECT id_expense_category FROM expense_categories WHERE name = 'Медицинские услуги')),
    ('Стоматология', (SELECT id_expense_category FROM expense_categories WHERE name = 'Медицинские услуги')),
    ('Страховка', (SELECT id_expense_category FROM expense_categories WHERE name = 'Мед'));

INSERT INTO expenses (amount, date_expense, description_expense, id_user, id_expense_item) VALUES
    (1800, DATE '2025-01-10', 'Покупка говядины',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Мясо')),
    (950, DATE '2025-01-12', 'Покупка рыбы',
        (SELECT id_user FROM users WHERE nickname = 'Мария'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Рыба')),
    (450, DATE '2025-01-15', 'Печенье к чаю',
        (SELECT id_user FROM users WHERE nickname = 'Алексей'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Сладости к чаю')),
    (2100, DATE '2026-01-09', 'Покупка мяса',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Мясо')),
    (5500, DATE '2026-01-14', 'Оплата электричества',
        (SELECT id_user FROM users WHERE nickname = 'Мария'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Электроэнергия')),
    (3500, DATE '2026-01-21', 'Консультация врача',
        (SELECT id_user FROM users WHERE nickname = 'Алексей'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Приём врача')),
    (1200, DATE '2026-05-08', 'Покупка рыбы',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Рыба')),
    (2100, DATE '2026-05-12', 'Оплата воды',
        (SELECT id_user FROM users WHERE nickname = 'Мария'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Водоснабжение')),
    (65000, DATE '2026-05-18', 'Лечение зубов',
        (SELECT id_user FROM users WHERE nickname = 'Алексей'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Стоматология')),
    (390, DATE '2026-09-11', 'Печенье «Курабье» к чаю',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Сладости к чаю')),
    (1600, DATE '2026-10-02', 'Покупка свинины',
        (SELECT id_user FROM users WHERE nickname = 'Иван'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Мясо')),
    (4800, DATE '2026-10-05', 'Оплата электричества',
        (SELECT id_user FROM users WHERE nickname = 'Мария'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Электроэнергия')),
    (2500, DATE '2026-10-09', 'Консультация терапевта',
        (SELECT id_user FROM users WHERE nickname = 'Алексей'),
        (SELECT id_expense_item FROM expense_items WHERE name = 'Приём врача'));

COMMIT;

-- Проверочные запросы а–ж.

-- а. Статьи расходов категорий, содержащих «мед», но не оканчивающихся на «мед».
SELECT ei.name AS expense_item
FROM expense_items AS ei
JOIN expense_categories AS ec ON ec.id_expense_category = ei.id_expense_category
WHERE ec.name ILIKE '%мед%'
  AND ec.name NOT ILIKE '%мед'
ORDER BY ei.name;

-- б. Пользователи с доходами разных категорий в одном месяце.
SELECT u.nickname, EXTRACT(YEAR FROM i.date_income)::int AS year,
       EXTRACT(MONTH FROM i.date_income)::int AS month
FROM incomes AS i
JOIN users AS u ON u.id_user = i.id_user
JOIN income_items AS ii ON ii.id_income_item = i.id_income_item
GROUP BY u.id_user, u.nickname, EXTRACT(YEAR FROM i.date_income), EXTRACT(MONTH FROM i.date_income)
HAVING COUNT(DISTINCT ii.id_income_category) > 1
ORDER BY year, month, u.nickname;

-- в. Статьи доходов без операций.
SELECT ii.name AS income_item
FROM income_items AS ii
WHERE NOT EXISTS (
    SELECT 1 FROM incomes AS i WHERE i.id_income_item = ii.id_income_item
)
ORDER BY ii.name;

-- г. Месяц максимального единоразового расхода текущего года.
SELECT EXTRACT(YEAR FROM e.date_expense)::int AS year,
       EXTRACT(MONTH FROM e.date_expense)::int AS month,
       e.amount AS max_single_expense
FROM expenses AS e
WHERE e.date_expense >= DATE_TRUNC('year', CURRENT_DATE)::date
  AND e.date_expense < (DATE_TRUNC('year', CURRENT_DATE) + INTERVAL '1 year')::date
ORDER BY e.amount DESC, e.date_expense
LIMIT 1;

-- д. Категории с доходами в январе любого года, но без доходов в мае 2026 года.
SELECT ic.name AS income_category
FROM income_categories AS ic
WHERE EXISTS (
    SELECT 1
    FROM income_items AS ii
    JOIN incomes AS i ON i.id_income_item = ii.id_income_item
    WHERE ii.id_income_category = ic.id_income_category
      AND EXTRACT(MONTH FROM i.date_income) = 1
)
AND NOT EXISTS (
    SELECT 1
    FROM income_items AS ii
    JOIN incomes AS i ON i.id_income_item = ii.id_income_item
    WHERE ii.id_income_category = ic.id_income_category
      AND i.date_income >= DATE '2026-05-01'
      AND i.date_income < DATE '2026-06-01'
)
ORDER BY ic.name;

-- е. Категории, по которым в январе 2025 года тратились все пользователи.
SELECT ec.name AS expense_category
FROM expense_categories AS ec
JOIN expense_items AS ei ON ei.id_expense_category = ec.id_expense_category
JOIN expenses AS e ON e.id_expense_item = ei.id_expense_item
WHERE e.date_expense >= DATE '2025-01-01'
  AND e.date_expense < DATE '2025-02-01'
GROUP BY ec.id_expense_category, ec.name
HAVING COUNT(DISTINCT e.id_user) = (SELECT COUNT(*) FROM users)
ORDER BY ec.name;

-- ж. Месяцы с числом различных статей расходов ниже среднего
-- по месяцам, в которых были расходы.
WITH monthly_items AS (
    SELECT DATE_TRUNC('month', e.date_expense)::date AS month_start,
           COUNT(DISTINCT e.id_expense_item) AS item_count
    FROM expenses AS e
    GROUP BY DATE_TRUNC('month', e.date_expense)::date
)
SELECT EXTRACT(YEAR FROM month_start)::int AS year,
       EXTRACT(MONTH FROM month_start)::int AS month,
       item_count
FROM monthly_items
WHERE item_count < (SELECT AVG(item_count) FROM monthly_items)
ORDER BY year, month;
