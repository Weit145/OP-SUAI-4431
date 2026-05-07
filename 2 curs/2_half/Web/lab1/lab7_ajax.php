<?php
header('Content-Type: application/json; charset=utf-8');

function value_from_post($field) {
    if (!isset($_POST[$field])) {
        return '';
    }

    if (is_array($_POST[$field])) {
        $values = array_map('trim', $_POST[$field]);
        $values = array_filter($values, static function($value) {
            return $value !== '';
        });
        return implode(', ', $values);
    }

    return trim((string)$_POST[$field]);
}

function json_response($payload) {
    echo json_encode($payload, JSON_UNESCAPED_UNICODE);
    exit;
}

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    json_response([
        'ok' => false,
        'errors' => ['Серверный сценарий принимает только POST-запрос.'],
    ]);
}

$fields = [
    ['label' => 'Фамилия', 'name' => 'surname', 'required' => true],
    ['label' => 'Имя', 'name' => 'name', 'required' => true],
    ['label' => 'Отчество', 'name' => 'patronymic', 'required' => true],
    ['label' => 'Цель посещения', 'name' => 'goal', 'required' => true],
    ['label' => 'Интересующие разделы', 'name' => 'topics', 'required' => true],
    ['label' => 'Уровень знакомства', 'name' => 'experience', 'required' => true],
    ['label' => 'Тема сайта', 'name' => 'site_theme', 'required' => false],
    ['label' => 'Вариант задания', 'name' => 'variant', 'required' => false],
];

$errors = [];
$responseFields = [];
$lineValues = [date('Y-m-d H:i:s')];

foreach ($fields as $field) {
    $value = value_from_post($field['name']);
    if ($field['required'] && $value === '') {
        $errors[] = 'Поле "' . $field['label'] . '" не заполнено.';
    }

    $safeValue = $value === '' ? '(не заполнено)' : $value;
    $responseFields[] = [
        'label' => htmlspecialchars($field['label'], ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8'),
        'value' => htmlspecialchars($safeValue, ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8'),
    ];
    $lineValues[] = $safeValue;
}

if (count($errors) > 0) {
    json_response([
        'ok' => false,
        'errors' => $errors,
    ]);
}

@file_put_contents(__DIR__ . '/ajax_results.txt', implode('; ', $lineValues) . PHP_EOL, FILE_APPEND);

json_response([
    'ok' => true,
    'message' => 'Ответ получен через XMLHttpRequest. Данные обработаны без перезагрузки страницы.',
    'fields' => $responseFields,
]);
