(function () {
    'use strict';

    const variants = {
        table: {
            xml: 'lab8_analytics.xml',
            xsl: 'lab8_analytics_table.xsl',
            title: 'Базовый XML: табличное отображение'
        },
        list: {
            xml: 'lab8_analytics.xml',
            xsl: 'lab8_analytics_list.xsl',
            title: 'Базовый XML: построчное отображение'
        },
        database: {
            xml: 'lab8_database.xml',
            xsl: 'lab8_database.xsl',
            title: 'XML на основе базы данных лабораторной №6'
        }
    };

    function loadXMLDoc(filename, callback, errorCallback) {
        const xhttp = new XMLHttpRequest();
        xhttp.open('GET', filename, true);
        try {
            xhttp.responseType = 'msxml-document';
        } catch (err) {
            // свойство нужно только для совместимости с IE
        }

        xhttp.onreadystatechange = function () {
            if (xhttp.readyState !== 4) return;

            if (xhttp.status !== 200) {
                errorCallback('Не удалось загрузить файл ' + filename + '. Код ответа: ' + xhttp.status);
                return;
            }

            let xmlDocument = xhttp.responseXML;
            if (!xmlDocument && xhttp.responseText) {
                xmlDocument = new DOMParser().parseFromString(xhttp.responseText, 'application/xml');
            }

            if (!xmlDocument || xmlDocument.getElementsByTagName('parsererror').length > 0) {
                errorCallback('Файл ' + filename + ' содержит ошибку XML-разметки.');
                return;
            }

            callback(xmlDocument);
        };

        xhttp.send(null);
    }

    function displayResult(mode) {
        const statusBox = document.getElementById('xml-status');
        const resultBox = document.getElementById('xml-result');
        const config = variants[mode] || variants.table;

        statusBox.className = 'ajax-status';
        statusBox.textContent = 'Загрузка: ' + config.xml + ' + ' + config.xsl + '...';
        resultBox.innerHTML = '';

        loadXMLDoc(config.xml, function (xml) {
            loadXMLDoc(config.xsl, function (xsl) {
                try {
                    if (window.ActiveXObject || xml.transformNode) {
                        resultBox.innerHTML = xml.transformNode(xsl);
                    } else if (document.implementation && document.implementation.createDocument) {
                        const xsltProcessor = new XSLTProcessor();
                        xsltProcessor.importStylesheet(xsl);
                        const resultDocument = xsltProcessor.transformToFragment(xml, document);
                        resultBox.innerHTML = '';
                        resultBox.appendChild(resultDocument);
                    }

                    statusBox.className = 'ajax-status ajax-status-ok';
                    statusBox.textContent = config.title + ' загружен успешно.';
                } catch (err) {
                    statusBox.className = 'ajax-status ajax-status-error';
                    statusBox.textContent = 'Ошибка XSLT-преобразования: ' + err.message;
                }
            }, function (message) {
                statusBox.className = 'ajax-status ajax-status-error';
                statusBox.textContent = message;
            });
        }, function (message) {
            statusBox.className = 'ajax-status ajax-status-error';
            statusBox.textContent = message;
        });
    }

    function normalizeMode(value) {
        const mode = (value || '').replace('#', '').trim();
        return variants[mode] ? mode : 'table';
    }

    document.addEventListener('DOMContentLoaded', function () {
        const buttons = document.querySelectorAll('[data-mode]');
        buttons.forEach(function (button) {
            button.addEventListener('click', function () {
                const mode = normalizeMode(button.getAttribute('data-mode'));
                window.location.hash = mode;
                displayResult(mode);
            });
        });

        window.addEventListener('hashchange', function () {
            displayResult(normalizeMode(window.location.hash));
        });

        displayResult(normalizeMode(window.location.hash));
    });
})();
