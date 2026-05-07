CREATE DATABASE IF NOT EXISTS lab6_analytics
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

CREATE USER IF NOT EXISTS 'lab6_user'@'localhost'
    IDENTIFIED BY 'lab6_pass';

GRANT ALL PRIVILEGES ON lab6_analytics.*
    TO 'lab6_user'@'localhost';

FLUSH PRIVILEGES;

USE lab6_analytics;

CREATE TABLE IF NOT EXISTS analytics_directions (
    direction_id INT AUTO_INCREMENT PRIMARY KEY,
    direction_name VARCHAR(120) NOT NULL UNIQUE,
    data_source VARCHAR(120) NOT NULL,
    main_method VARCHAR(120) NOT NULL,
    business_goal VARCHAR(160) NOT NULL,
    difficulty_level VARCHAR(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS analysis_projects (
    project_id INT AUTO_INCREMENT PRIMARY KEY,
    direction_id INT NOT NULL,
    project_name VARCHAR(140) NOT NULL,
    metric_name VARCHAR(120) NOT NULL,
    tool_name VARCHAR(80) NOT NULL,
    dataset_size VARCHAR(80) NOT NULL,
    result_summary VARCHAR(220) NOT NULL,
    responsible VARCHAR(120) NOT NULL,
    CONSTRAINT fk_project_direction
        FOREIGN KEY (direction_id)
        REFERENCES analytics_directions(direction_id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

INSERT IGNORE INTO analytics_directions
    (direction_name, data_source, main_method, business_goal, difficulty_level)
VALUES
    ('Описательная аналитика', 'Анкеты пользователей', 'Сводные статистики', 'Понимание структуры данных', 'Начальный'),
    ('Прогнозная аналитика', 'Исторические наборы данных', 'Регрессионные модели', 'Прогноз показателей', 'Средний'),
    ('Визуальная аналитика', 'BI-отчеты и дашборды', 'Интерактивная визуализация', 'Быстрое принятие решений', 'Базовый');

INSERT INTO analysis_projects
    (direction_id, project_name, metric_name, tool_name, dataset_size, result_summary, responsible)
SELECT direction_id, 'Анализ посещаемости сайта', 'Среднее время просмотра', 'Excel', '1200 строк', 'Выявлены популярные разделы', 'Кочнев Д. О.'
FROM analytics_directions
WHERE direction_name = 'Описательная аналитика'
  AND NOT EXISTS (SELECT 1 FROM analysis_projects WHERE project_name = 'Анализ посещаемости сайта');

INSERT INTO analysis_projects
    (direction_id, project_name, metric_name, tool_name, dataset_size, result_summary, responsible)
SELECT direction_id, 'Анкета интересов пользователей', 'Доля выбранных тем', 'Python', '300 ответов', 'Определены основные интересы аудитории', 'Кочнев Д. О.'
FROM analytics_directions
WHERE direction_name = 'Описательная аналитика'
  AND NOT EXISTS (SELECT 1 FROM analysis_projects WHERE project_name = 'Анкета интересов пользователей');

INSERT INTO analysis_projects
    (direction_id, project_name, metric_name, tool_name, dataset_size, result_summary, responsible)
SELECT direction_id, 'Прогноз роста данных', 'Месячный прирост', 'R', '24 месяца', 'Построен прогноз нагрузки', 'Кочнев Д. О.'
FROM analytics_directions
WHERE direction_name = 'Прогнозная аналитика'
  AND NOT EXISTS (SELECT 1 FROM analysis_projects WHERE project_name = 'Прогноз роста данных');

INSERT INTO analysis_projects
    (direction_id, project_name, metric_name, tool_name, dataset_size, result_summary, responsible)
SELECT direction_id, 'Дашборд качества данных', 'Процент пропусков', 'Power BI', '8 таблиц', 'Собраны ключевые показатели качества', 'Кочнев Д. О.'
FROM analytics_directions
WHERE direction_name = 'Визуальная аналитика'
  AND NOT EXISTS (SELECT 1 FROM analysis_projects WHERE project_name = 'Дашборд качества данных');
