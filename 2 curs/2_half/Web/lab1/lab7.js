(function() {
    document.addEventListener('DOMContentLoaded', function() {
        const form = document.getElementById('ajax-survey-form');
        const statusBox = document.getElementById('ajax-status');
        const responseBox = document.getElementById('ajax-response');

        if (!form || !statusBox || !responseBox) return;

        function encodeForm(formElement) {
            const pairs = [];
            const elements = formElement.elements;

            for (let i = 0; i < elements.length; i++) {
                const element = elements[i];
                if (!element.name || element.disabled) continue;
                if ((element.type === 'checkbox' || element.type === 'radio') && !element.checked) continue;
                pairs.push(encodeURIComponent(element.name) + '=' + encodeURIComponent(element.value));
            }

            return pairs.join('&');
        }

        function showErrors(errors) {
            statusBox.className = 'ajax-status ajax-status-error';
            statusBox.textContent = errors.join(' ');
            responseBox.innerHTML = '';
        }

        function renderResponse(data) {
            let html = '<dl class="response-list">';
            for (let i = 0; i < data.fields.length; i++) {
                html += '<dt>' + data.fields[i].label + '</dt>';
                html += '<dd>' + data.fields[i].value + '</dd>';
            }
            html += '</dl>';

            statusBox.className = 'ajax-status ajax-status-ok';
            statusBox.textContent = data.message;
            responseBox.innerHTML = html;
        }

        form.addEventListener('submit', function(event) {
            event.preventDefault();

            statusBox.className = 'ajax-status';
            statusBox.textContent = 'Запрос отправлен, страница не перезагружается.';
            responseBox.innerHTML = '';

            const request = new XMLHttpRequest();
            request.onreadystatechange = function() {
                if (request.readyState !== 4) return;

                if (request.status !== 200) {
                    showErrors(['Ошибка сервера: ' + request.status + '.']);
                    return;
                }

                try {
                    const data = JSON.parse(request.responseText);
                    if (!data.ok) {
                        showErrors(data.errors || ['Сервер вернул ошибку.']);
                        return;
                    }
                    renderResponse(data);
                } catch (error) {
                    showErrors(['Ответ сервера не удалось разобрать как JSON.']);
                }
            };

            request.open('POST', form.action, true);
            request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8');
            request.setRequestHeader('X-Requested-With', 'XMLHttpRequest');
            request.send(encodeForm(form));
        });

        form.addEventListener('reset', function() {
            statusBox.className = 'ajax-status';
            statusBox.textContent = 'Ожидание отправки формы.';
            responseBox.innerHTML = '';
        });
    });
})();
