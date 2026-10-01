const app = angular.module('properties', []);

app.config(function ($httpProvider) {
    const token = document.querySelector('meta[name="_csrf"]').content;
    const header = document.querySelector('meta[name="_csrf_header"]').content;
    $httpProvider.defaults.headers.common[header] = token;
});

app.controller('PropertiesController', function ($scope, $http) {
    $scope.properties = [];
    $scope.audit = [];
    $scope.form = {type: 'APARTMENT', available: true};

    function showError(error) {
        const details = error.data && error.data.validationErrors;
        $scope.error = details && Object.keys(details).length
            ? Object.values(details).join('; ')
            : (error.status === 401 ? 'Войдите в систему для изменения данных.'
                : error.status === 403 ? 'Доступ запрещён или истёк CSRF-токен. Обновите страницу.'
                : 'Не удалось выполнить запрос.');
    }

    $scope.load = function () {
        $http.get('/api/properties').then(function (response) {
            $scope.properties = response.data;
            $scope.error = '';
        }, showError);
    };

    $scope.loadAudit = function () {
        $http.get('/api/audit').then(function (response) {
            $scope.audit = response.data;
        }, showError);
    };

    $scope.clear = function () {
        $scope.form = {type: 'APARTMENT', available: true};
    };

    $scope.edit = function (property) {
        $scope.form = angular.copy(property);
        window.scrollTo(0, document.body.scrollHeight);
    };

    $scope.save = function () {
        const body = angular.copy($scope.form);
        const request = body.id
            ? $http.put('/api/properties/' + body.id, body)
            : $http.post('/api/properties', body);
        request.then(function () {
            $scope.clear();
            $scope.load();
            $scope.loadAudit();
        }, showError);
    };

    $scope.remove = function (property) {
        if (!window.confirm('Удалить объект «' + property.address + '»?')) return;
        $http.delete('/api/properties/' + property.id).then(function () {
            $scope.load();
            $scope.loadAudit();
        }, showError);
    };

    $scope.load();
    if (document.getElementById('editor-marker')) $scope.loadAudit();
});
