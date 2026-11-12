-- а) Статьи расходов категорий, название которых содержит «мед»,
-- но не заканчивается на него.

SELECT c.name AS category_name, item.name AS expense_item
FROM expense_categories AS c
JOIN expense_items AS item
    ON item.id_expense_category = c.id_expense_category
WHERE c.name iLIKE '%Мед_%'
ORDER BY c.name, item.name;

-- б) Пользователь, у которого было несколько доходов разных категорий
-- в одном месяце одного года.

SELECT DISTINCT u.id_user, u.nickname
FROM users AS u
JOIN incomes AS i1 ON i1.id_user = u.id_user
JOIN incomes AS i2
    ON i2.id_user = u.id_user
    AND EXTRACT(YEAR FROM i1.date_income) = EXTRACT(YEAR FROM i2.date_income)
    AND EXTRACT(MONTH FROM i1.date_income) = EXTRACT(MONTH FROM i2.date_income)
JOIN income_items AS item1 ON item1.id_income_item = i1.id_income_item
JOIN income_items AS item2 ON item2.id_income_item = i2.id_income_item
WHERE item1.id_income_category <> item2.id_income_category
    AND i1.id_income <> i2.id_income
ORDER BY u.nickname;

-- в) Статья доходов без доходов по ней.
SELECT item.name AS income_item
FROM income_items AS item
LEFT JOIN incomes AS i ON i.id_income_item = item.id_income_item
WHERE i.id_income IS NULL
ORDER BY item.name;

SELECT item.name AS income_item
FROM incomes AS i
RIGHT JOIN income_items AS item ON i.id_income_item = item.id_income_item
WHERE i.id_income IS NULL
ORDER BY item.name;
