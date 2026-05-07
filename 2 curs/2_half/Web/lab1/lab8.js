(function() {
    document.addEventListener('DOMContentLoaded', function() {
        const button = document.getElementById('loadXmlButton');
        const statusBox = document.getElementById('xml-status');
        const tableBody = document.getElementById('xml-projects-body');

        if (!button || !statusBox || !tableBody) return;

        function textFrom(project, tagName) {
            const elements = project.getElementsByTagName(tagName);
            if (elements.length === 0 || !elements[0].textContent) {
                return '';
            }
            return elements[0].textContent;
        }

        function addCell(row, text) {
            const cell = document.createElement('td');
            cell.textContent = text;
            row.appendChild(cell);
        }

        function renderProjects(xmlDocument) {
            const projects = xmlDocument.getElementsByTagName('project');
            tableBody.innerHTML = '';

            if (projects.length === 0) {
                const row = document.createElement('tr');
                addCell(row, 'В XML-файле нет элементов project.');
                row.firstChild.colSpan = 7;
                tableBody.appendChild(row);
                return;
            }

            for (let i = 0; i < projects.length; i++) {
                const project = projects[i];
                const row = document.createElement('tr');

                addCell(row, project.getAttribute('id') || String(i + 1));
                addCell(row, textFrom(project, 'direction'));
                addCell(row, textFrom(project, 'dataSource'));
                addCell(row, textFrom(project, 'method'));
                addCell(row, textFrom(project, 'tool'));
                addCell(row, textFrom(project, 'metric'));
                addCell(row, textFrom(project, 'result'));

                tableBody.appendChild(row);
            }

            statusBox.className = 'ajax-status ajax-status-ok';
            statusBox.textContent = 'XML-файл загружен. Записей: ' + projects.length + '.';
        }

        function loadXml() {
            statusBox.className = 'ajax-status';
            statusBox.textContent = 'Загрузка lab8_data.xml...';

            const request = new XMLHttpRequest();
            request.onreadystatechange = function() {
                if (request.readyState !== 4) return;

                if (request.status !== 200) {
                    statusBox.className = 'ajax-status ajax-status-error';
                    statusBox.textContent = 'Ошибка загрузки XML: ' + request.status + '.';
                    return;
                }

                const xmlDocument = request.responseXML || new DOMParser().parseFromString(request.responseText, 'application/xml');
                const parserError = xmlDocument.getElementsByTagName('parsererror');
                if (parserError.length > 0) {
                    statusBox.className = 'ajax-status ajax-status-error';
                    statusBox.textContent = 'XML-файл содержит ошибку разметки.';
                    return;
                }

                renderProjects(xmlDocument);
            };

            request.open('GET', 'lab8_data.xml', true);
            request.send(null);
        }

        button.addEventListener('click', loadXml);
        loadXml();
    });
})();
