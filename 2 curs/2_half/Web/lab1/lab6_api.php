<?php
header('Content-Type: application/json; charset=utf-8');
mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);

const DB_HOST = 'localhost';
const DB_USER = 'lab6_user';
const DB_PASSWORD = 'lab6_pass';
const DB_NAME = 'lab6_analytics';

function h($value) {
    return htmlspecialchars((string)$value, ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8');
}

function clean($value) {
    return trim((string)$value);
}

function json_response($payload) {
    echo json_encode($payload, JSON_UNESCAPED_UNICODE);
    exit;
}

function db_connect() {
    $link = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);
    mysqli_set_charset($link, 'utf8mb4');
    initialize_db($link);
    return $link;
}

function table_count($link, $tableName) {
    $result = mysqli_query($link, "SELECT COUNT(*) FROM `$tableName`");
    $row = mysqli_fetch_row($result);
    mysqli_free_result($result);
    return (int)$row[0];
}

function initialize_db($link) {
    mysqli_query($link, "
        CREATE TABLE IF NOT EXISTS analytics_directions (
            direction_id INT AUTO_INCREMENT PRIMARY KEY,
            direction_name VARCHAR(120) NOT NULL UNIQUE,
            data_source VARCHAR(120) NOT NULL,
            main_method VARCHAR(120) NOT NULL,
            business_goal VARCHAR(160) NOT NULL,
            difficulty_level VARCHAR(40) NOT NULL
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    ");

    mysqli_query($link, "
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
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    ");

    if (table_count($link, 'analytics_directions') === 0) {
        $directions = [
            ['Описательная аналитика', 'Анкеты пользователей', 'Сводные статистики', 'Понимание структуры данных', 'Начальный'],
            ['Прогнозная аналитика', 'Исторические наборы данных', 'Регрессионные модели', 'Прогноз показателей', 'Средний'],
            ['Визуальная аналитика', 'BI-отчеты и дашборды', 'Интерактивная визуализация', 'Быстрое принятие решений', 'Базовый'],
        ];

        $stmt = mysqli_prepare($link, "
            INSERT INTO analytics_directions
                (direction_name, data_source, main_method, business_goal, difficulty_level)
            VALUES (?, ?, ?, ?, ?)
        ");
        $directionName = '';
        $dataSource = '';
        $mainMethod = '';
        $businessGoal = '';
        $difficultyLevel = '';
        mysqli_stmt_bind_param($stmt, 'sssss', $directionName, $dataSource, $mainMethod, $businessGoal, $difficultyLevel);

        foreach ($directions as $direction) {
            [$directionName, $dataSource, $mainMethod, $businessGoal, $difficultyLevel] = $direction;
            mysqli_stmt_execute($stmt);
        }
        mysqli_stmt_close($stmt);
    }
}

function fetch_directions($link) {
    $directions = [];
    $result = mysqli_query($link, "
        SELECT direction_id, direction_name, data_source, main_method, business_goal, difficulty_level
        FROM analytics_directions
        ORDER BY direction_name
    ");

    while ($row = mysqli_fetch_assoc($result)) {
        $directions[] = $row;
    }
    mysqli_free_result($result);
    return $directions;
}

function fetch_projects($link) {
    $projects = [];
    $result = mysqli_query($link, "
        SELECT
            p.project_id,
            p.direction_id,
            d.direction_name,
            d.data_source,
            d.main_method,
            d.business_goal,
            d.difficulty_level,
            p.project_name,
            p.metric_name,
            p.tool_name,
            p.dataset_size,
            p.result_summary,
            p.responsible
        FROM analysis_projects p
        INNER JOIN analytics_directions d ON d.direction_id = p.direction_id
        ORDER BY d.direction_name, p.project_name
    ");

    while ($row = mysqli_fetch_assoc($result)) {
        $projects[] = $row;
    }
    mysqli_free_result($result);
    return $projects;
}

function direction_options($directions, $selectedId = null) {
    $html = '<option value="">Выберите направление</option>';
    foreach ($directions as $direction) {
        $selected = ((int)$direction['direction_id'] === (int)$selectedId) ? ' selected' : '';
        $html .= '<option value="' . (int)$direction['direction_id'] . '"' . $selected . '>' . h($direction['direction_name']) . '</option>';
    }
    return $html;
}

function handle_add_projects($link) {
    $messages = [];
    $errors = [];
    $projects = $_POST['projects'] ?? [];

    if (!is_array($projects)) {
        return [[], ['Данные формы добавления повреждены.']];
    }

    $stmt = mysqli_prepare($link, "
        INSERT INTO analysis_projects
            (direction_id, project_name, metric_name, tool_name, dataset_size, result_summary, responsible)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    ");
    $directionId = 0;
    $projectName = '';
    $metricName = '';
    $toolName = '';
    $datasetSize = '';
    $resultSummary = '';
    $responsible = '';
    mysqli_stmt_bind_param($stmt, 'issssss', $directionId, $projectName, $metricName, $toolName, $datasetSize, $resultSummary, $responsible);

    $added = 0;
    foreach ($projects as $number => $project) {
        if (!is_array($project)) {
            continue;
        }

        $directionId = (int)($project['direction_id'] ?? 0);
        $projectName = clean($project['project_name'] ?? '');
        $metricName = clean($project['metric_name'] ?? '');
        $toolName = clean($project['tool_name'] ?? '');
        $datasetSize = clean($project['dataset_size'] ?? '');
        $resultSummary = clean($project['result_summary'] ?? '');
        $responsible = clean($project['responsible'] ?? '');

        $allEmpty = $directionId === 0
            && $projectName === ''
            && $metricName === ''
            && $toolName === ''
            && $datasetSize === ''
            && $resultSummary === ''
            && $responsible === '';

        if ($allEmpty) {
            continue;
        }

        if ($directionId === 0 || $projectName === '' || $metricName === '' || $toolName === '' || $datasetSize === '' || $resultSummary === '' || $responsible === '') {
            $errors[] = 'Строка ' . ((int)$number + 1) . ' не добавлена: заполните все поля.';
            continue;
        }

        mysqli_stmt_execute($stmt);
        $added++;
    }
    mysqli_stmt_close($stmt);

    if ($added > 0) {
        $messages[] = 'Добавлено строк через fetch(): ' . $added . '.';
    }
    if ($added === 0 && count($errors) === 0) {
        $errors[] = 'Нет заполненных строк для добавления.';
    }

    return [$messages, $errors];
}

function handle_update_project($link) {
    $projectId = (int)($_POST['project_id'] ?? 0);
    $directionId = (int)($_POST['direction_id'] ?? 0);
    $projectName = clean($_POST['project_name'] ?? '');
    $metricName = clean($_POST['metric_name'] ?? '');
    $toolName = clean($_POST['tool_name'] ?? '');
    $datasetSize = clean($_POST['dataset_size'] ?? '');
    $resultSummary = clean($_POST['result_summary'] ?? '');
    $responsible = clean($_POST['responsible'] ?? '');

    if ($projectId === 0 || $directionId === 0 || $projectName === '' || $metricName === '' || $toolName === '' || $datasetSize === '' || $resultSummary === '' || $responsible === '') {
        return [[], ['Редактирование не выполнено: заполните все поля.']];
    }

    $stmt = mysqli_prepare($link, "
        UPDATE analysis_projects
        SET direction_id = ?,
            project_name = ?,
            metric_name = ?,
            tool_name = ?,
            dataset_size = ?,
            result_summary = ?,
            responsible = ?
        WHERE project_id = ?
    ");
    mysqli_stmt_bind_param($stmt, 'issssssi', $directionId, $projectName, $metricName, $toolName, $datasetSize, $resultSummary, $responsible, $projectId);
    mysqli_stmt_execute($stmt);
    mysqli_stmt_close($stmt);

    return [['Строка обновлена через fetch().'], []];
}

function handle_delete_project($link) {
    $projectId = (int)($_POST['project_id'] ?? 0);
    if ($projectId === 0) {
        return [[], ['Удаление не выполнено: строка не найдена.']];
    }

    $stmt = mysqli_prepare($link, "DELETE FROM analysis_projects WHERE project_id = ?");
    mysqli_stmt_bind_param($stmt, 'i', $projectId);
    mysqli_stmt_execute($stmt);
    mysqli_stmt_close($stmt);

    return [['Строка удалена через fetch().'], []];
}

function handle_request($link) {
    if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
        return [[], []];
    }

    $action = $_POST['action'] ?? '';
    if ($action === 'add_projects') {
        return handle_add_projects($link);
    }
    if ($action === 'update_project') {
        return handle_update_project($link);
    }
    if ($action === 'delete_project') {
        return handle_delete_project($link);
    }
    return [[], ['Неизвестное действие формы.']];
}

function render_project_tbody($projects, $directions) {
    ob_start();

    if (count($projects) === 0) {
        echo '<tr><td colspan="12">В базе пока нет проектных строк.</td></tr>';
    }

    foreach ($projects as $project) {
        $editForm = '<details class="edit-panel"><summary>Редактировать</summary>';
        $editForm .= '<form method="post" action="lab6.php">';
        $editForm .= '<input type="hidden" name="action" value="update_project">';
        $editForm .= '<input type="hidden" name="project_id" value="' . (int)$project['project_id'] . '">';
        $editForm .= '<label>Направление<select name="direction_id">' . direction_options($directions, $project['direction_id']) . '</select></label>';
        $editForm .= '<label>Проект<input type="text" name="project_name" value="' . h($project['project_name']) . '"></label>';
        $editForm .= '<label>Метрика<input type="text" name="metric_name" value="' . h($project['metric_name']) . '"></label>';
        $editForm .= '<label>Инструмент<input type="text" name="tool_name" value="' . h($project['tool_name']) . '"></label>';
        $editForm .= '<label>Объем данных<input type="text" name="dataset_size" value="' . h($project['dataset_size']) . '"></label>';
        $editForm .= '<label>Результат<input type="text" name="result_summary" value="' . h($project['result_summary']) . '"></label>';
        $editForm .= '<label>Ответственный<input type="text" name="responsible" value="' . h($project['responsible']) . '"></label>';
        $editForm .= '<button class="primary-button" type="submit">Сохранить</button>';
        $editForm .= '</form></details>';
        $editForm .= '<form method="post" action="lab6.php" class="delete-form" data-confirm="Удалить строку?">';
        $editForm .= '<input type="hidden" name="action" value="delete_project">';
        $editForm .= '<input type="hidden" name="project_id" value="' . (int)$project['project_id'] . '">';
        $editForm .= '<button class="secondary-button" type="submit">Удалить</button>';
        $editForm .= '</form>';

        printf(
            '<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>',
            h($project['direction_name']),
            h($project['data_source']),
            h($project['main_method']),
            h($project['business_goal']),
            h($project['difficulty_level']),
            h($project['project_name']),
            h($project['metric_name']),
            h($project['tool_name']),
            h($project['dataset_size']),
            h($project['result_summary']),
            h($project['responsible']),
            $editForm
        );
    }

    return ob_get_clean();
}

try {
    $link = db_connect();
    [$messages, $errors] = handle_request($link);
    $directions = fetch_directions($link);
    $projects = fetch_projects($link);
    $tbody = render_project_tbody($projects, $directions);
    mysqli_close($link);

    json_response([
        'ok' => count($errors) === 0,
        'messages' => $messages,
        'errors' => $errors,
        'tbody' => $tbody,
    ]);
} catch (Throwable $exception) {
    json_response([
        'ok' => false,
        'messages' => [],
        'errors' => ['Ошибка базы данных: ' . $exception->getMessage()],
        'tbody' => '',
    ]);
}
