CREATE TABLE IF NOT EXISTS users (
    id_user UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    nickname VARCHAR(256) NOT NULL
);


-- Income

CREATE TABLE IF NOT EXISTS income_categories (
    id_income_category UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS income_items (
    id_income_item UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(256) NOT NULL,
    id_income_category UUID NOT NULL,
    CONSTRAINT fk_income_categories FOREIGN KEY (id_income_category) REFERENCES income_categories(id_income_category)
);


CREATE TABLE IF NOT EXISTS incomes (
    id_income UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    amount DECIMAL(12, 2) NOT NULL,
    date_income DATE NOT NULL,
    description_income VARCHAR(256), -- TEXT
    id_user UUID NOT NULL,
    id_income_item UUID NOT NULL,
    CONSTRAINT fk_users FOREIGN KEY (id_user) REFERENCES users(id_user),
    CONSTRAINT fk_income_items FOREIGN KEY (id_income_item) REFERENCES income_items(id_income_item)
);


-- Expenses

CREATE TABLE IF NOT EXISTS expense_categories (
    id_expense_category UUID NOT NULL PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS expense_items (
    id_expense_item UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(256) NOT NULL,
    id_expense_category UUID NOT NULL,
    CONSTRAINT fk_expense_categories FOREIGN KEY (id_expense_category) REFERENCES expense_categories(id_expense_category)
);

CREATE TABLE IF NOT EXISTS expenses (
    id_expense UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    amount DECIMAL(12, 2) NOT NULL,
    date_expense DATE NOT NULL,
    description_expense VARCHAR(256), -- TEXT
    id_user UUID NOT NULL,
    id_expense_item UUID NOT NULL,
    CONSTRAINT fk_users FOREIGN KEY (id_user) REFERENCES users(id_user),
    CONSTRAINT fk_expense_items FOREIGN KEY (id_expense_item) REFERENCES expense_items(id_expense_item)
);
