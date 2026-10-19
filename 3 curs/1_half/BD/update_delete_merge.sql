UPDATE incomes SET amount = 90000, description_income = 'Зарплата за май с учётом перерасчёта'
FROM users , income_items
WHERE incomes.id_user = users.id_user
  AND incomes.id_income_item = income_items.id_income_item
  AND users.nickname = 'Иван'
  AND income_items.name = 'Оклад'
  AND incomes.date_income = DATE '2026-05-06';

DELETE FROM expenses
WHERE date_expense < DATE '2026-01-01';




CREATE TABLE IF NOT EXISTS expense_categories_new (
    id_expense_category UUID NOT NULL PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(256) NOT NULL
);

INSERT INTO expense_categories_new (name) VALUES
    ('Образование');


MERGE INTO expense_categories
USING expense_categories_new ON expense_categories.id = expense_categories_new.id
WHEN MATCHED THEN
    UPDATE SET name = expense_categories_new.name
WHEN NOT MATCHED THEN
    INSERT (name) VALUES (expense_categories_new.name);
