#!/usr/bin/env python3
"""Build the editable report source using the Lab 6 cover and verified results."""
import html
import json
import re
from pathlib import Path

root = Path(__file__).resolve().parents[1]
template = (root / "ОтчетЛР6Кочнев4431.html").read_text(encoding="utf-8")
cover = template.split('<h2 class="assignment">')[0]
cover = cover.replace("№6", "№7").replace(
    "РАЗРАБОТКА ПРИЛОЖЕНИЯ С ИСПОЛЬЗОВАНИЕМ HIBERNATE",
    "РАЗРАБОТКА ПРИЛОЖЕНИЯ С АСИНХРОННОЙ ОЧЕРЕДЬЮ СООБЩЕНИЙ")
cover = cover.replace(
    ".assignment { page-break-before: always; }",
    """.assignment { page-break-before: always; }
.cover-header { line-height: 1.25; }
.approval { margin-top: 1.1cm; }
.approval-title { margin-bottom: 0.35cm; }
.role { margin-top: 0.3cm; }
.signature-line { margin-top: 0.25cm; white-space: nowrap; }
.signature-hint { margin-top: 0.05cm; font-size: 9pt; white-space: nowrap; }
.main-title { margin-top: 2.0cm; font-weight: bold; }
.work-title { margin-top: 0.55cm; font-weight: bold; line-height: 1.25; }
.course { margin-top: 0.35cm; }
.student { margin-top: 1.25cm; }
.city { margin-top: 4.3cm; }""")
new_cover = """<div class="cover">
  <p class="cover-header">ГУАП<br>КАФЕДРА № 43</p>

  <div class="approval">
    <p class="approval-title">ОТЧЕТ ЗАЩИЩЕН С ОЦЕНКОЙ __________________</p>
    <p class="role">ПРЕПОДАВАТЕЛЬ</p>
    <p class="signature-line">Доцент, канд. техн. наук&nbsp;&nbsp;&nbsp;&nbsp;________________&nbsp;&nbsp;&nbsp;&nbsp;П. А. Степанов</p>
    <p class="signature-hint">должность, уч. степень, звание&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись, дата&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;инициалы, фамилия</p>
  </div>

  <p class="main-title">ОТЧЕТ О ЛАБОРАТОРНОЙ РАБОТЕ №7</p>
  <p class="work-title">РАЗРАБОТКА ПРИЛОЖЕНИЯ<br>С АСИНХРОННОЙ ОЧЕРЕДЬЮ СООБЩЕНИЙ</p>
  <p class="course">по курсу: Технология разработки серверных информационных систем</p>

  <div class="student">
    <p class="role">РАБОТУ ВЫПОЛНИЛ</p>
    <p class="signature-line">СТУДЕНТ гр. № 4431&nbsp;&nbsp;&nbsp;&nbsp;________________&nbsp;&nbsp;&nbsp;&nbsp;Д. О. Кочнев</p>
    <p class="signature-hint">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись, дата&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;инициалы, фамилия</p>
  </div>

  <p class="city">Санкт-Петербург 2026</p>
</div>"""
cover_start = cover.index('<div class="cover">')
cover_end = cover.rfind("</div>") + len("</div>")
cover = cover[:cover_start] + new_cover + "\n" + cover[cover_end:]
parts = [cover]


def paragraph(text):
    parts.append("<p>" + html.escape(text) + "</p>")


def heading(text):
    parts.append("<h2>" + html.escape(text) + "</h2>")


def code(text):
    for line in text.strip("\n").splitlines():
        parts.append('<p style="font-family:Courier New;font-size:9pt;line-height:100%;'
                     'margin:0;text-align:left">' +
                     (html.escape(line).replace(" ", "&nbsp;") or "&nbsp;") + "</p>")
    parts.append("<p></p>")


parts.append('<h2 class="assignment">Задание:</h2>')
paragraph("Цель работы — реализация простой системы распределённой репликации по модели «писатели — читатели».")
parts.append("<ol><li>Развернуть Apache Kafka.</li>"
             "<li>Обеспечить запуск нескольких экземпляров приложения со встроенной базой данных на разных портах.</li>"
             "<li>Реализовать Producer, отправляющий события каждой операции записи, и Consumer, применяющий изменения к локальной базе.</li>"
             "<li>Продемонстрировать доступность записанных данных в другом экземпляре.</li></ol>")
heading("Описание разрабатываемого продукта:")
paragraph("Вариант 14 — «Сдача недвижимости в аренду». Работа выполнена на основе server_app из лабораторных №4 и №6. В приложении используются Spring Boot, Spring Security, HTTPS, Thymeleaf, AngularJS, Spring Data JPA, H2 и Liquibase. Добавлены Spring Kafka, Producer и Consumer, реплицирующие объекты недвижимости и журнал аудита.")
paragraph("Выбран основной вариант задания с двумя встроенными базами H2. Пример преподавателя lab78example использует альтернативный вариант с PostgreSQL, Kafka Connect и Debezium; в данной работе изменения публикует само приложение.")
heading("Архитектура и порядок работы:")
code("Экземпляр 1 (HTTPS 8443, H2 №1)\n           ↕ Producer / Consumer\n     Kafka: property-changes, 1 партиция\n           ↕ Producer / Consumer\nЭкземпляр 2 (HTTPS 9443, H2 №2)")
paragraph("Каждый экземпляр имеет собственную базу H2 и отдельную группу потребителей Kafka. Поэтому каждое событие получает каждый экземпляр, а не только один участник общей группы. Одна партиция задаёт одинаковый порядок применения событий на всех репликах.")
paragraph("После создания, изменения или удаления объекта сервис записывает событие в REPLICATION_OUTBOX в той же транзакции, что и локальные данные. Планировщик отправляет зафиксированное событие в Kafka и удаляет его из outbox только после подтверждения брокера. Consumer применяет изменение, записывает аудит и UUID события в REPLICATION_APPLIED в одной транзакции.")
paragraph("Повторно доставленное событие пропускается по UUID. Consumer не вызывает методы, формирующие новые события, что предотвращает зацикливание. Собственные события также применяются в порядке Kafka: при конкурирующих изменениях итоговое состояние определяется последним событием в журнале.")
paragraph("Событие содержит UUID, имя источника, действие CREATE/UPDATE/DELETE, логин пользователя, время и полный снимок объекта. Связь PROPERTY → USERS сохраняется: пользователь определяется по логину. Учебная учётная запись guest создаётся в каждой базе из data.sql.")
heading("Код программы (основные фрагменты):")
paragraph("1. Контракт события (PropertyChange.java):")
code("public record PropertyChange(String eventId, String source,\n        String action, String actor, Instant occurredAt,\n        PropertyDTO property) {\n}")
paragraph("2. Настройка Kafka и профиля репликации:")
code("app.replication.enabled=true\nserver.servlet.session.cookie.name=SESSION_${app.instance-id}\nspring.kafka.bootstrap-servers=${KAFKA_BOOTSTRAP_SERVERS:localhost:9092}\nspring.kafka.consumer.auto-offset-reset=earliest\nspring.kafka.consumer.enable-auto-commit=false\nspring.kafka.listener.ack-mode=record")
paragraph("При каждом запуске группа формируется как properties-<instance-id>-<UUID>. Это необходимо, поскольку H2 в памяти создаётся заново и должна перечитать журнал с начала. Разные имена cookie позволяют войти в два экземпляра из одного браузера.")
paragraph("3. Регистрация исходящего события в транзакции (ReplicationEvents.java):")
source = (root / "main/java/info/stepanoff/trsis/samples/replication/ReplicationEvents.java").read_text()
code(source[source.index("    @Transactional"):source.rfind("}")])
paragraph("4. Отправка через KafkaTemplate (OutboxPublisher.java):")
source = (root / "main/java/info/stepanoff/trsis/samples/replication/OutboxPublisher.java").read_text()
code(source[source.index("    @Scheduled"):source.rfind("}")])
paragraph("5. Основные шаги Consumer (PropertyChangeConsumer.java):")
code('@Transactional\n@KafkaListener(topics = "${app.replication.topic}",\n        groupId = "#{@replicationGroup}")\npublic void receive(String json) throws JsonProcessingException {\n    PropertyChange event = mapper.readValue(json, PropertyChange.class);\n    if (applied.existsById(event.eventId())) return;\n    // DELETE: удалить объект, если он существует.\n    // CREATE/UPDATE: найти или создать объект по ID,\n    // перенести поля DTO, найти пользователя и вызвать save().\n    // Записать аудит с исходными actor, action, occurredAt.\n    applied.save(new AppliedEvent(event.eventId()));\n}')
paragraph("Фрагменты с комментариями сокращены; полный код расположен в пакете replication. Применение данных и отметка обработки фиксируются вместе, а смещение Kafka подтверждается после успешной обработки записи. При ошибке обработка повторяется.")
paragraph("6. Идентификаторы объектов (PropertyServiceImpl.java):")
code("entity.setId(UUID.randomUUID().getMostSignificantBits()\n        & 0x001fffffffffffffL);")
paragraph("Независимые счётчики двух баз могли бы создать один и тот же ID. Новые ID генерируются случайно в пределах точных целых чисел JavaScript, до 2^53−1. Вероятность случайного совпадения мала, но не равна нулю. Начальные записи имеют одинаковые ID в обеих базах.")
heading("Запуск и демонстрация:")
code("make kafka\nmake build\n# Первый терминал:\nmake run1\n# Второй терминал:\nmake run2")
paragraph("Адреса: https://localhost:8443/ и https://localhost:9443/. Учебный логин: guest, пароль: hello. Данные страницы обновляются каждые две секунды. Брокер запускается в Docker в режиме KRaft, без ZooKeeper; его данные сохраняются в отдельном томе.")
heading("Результаты проверки:")
for path in sorted((root / "verification").glob("*.txt")):
    match = re.search(r"Tests run:.*", path.read_text())
    if match:
        code(match.group())
paragraph("Существующие шесть тестов проверяют REST CRUD, валидацию, авторизацию, CSRF, вход/выход и внешний ключ. Интеграционный тест с Kafka проверяет обмен между двумя независимыми H2 в обе стороны, удаление, повторную доставку одного события и восстановление второй реплики после перезапуска.")
result = json.loads((root / "verification/https-replication.json").read_text())
assert result["result"] == "PASS"
paragraph("Дополнительно выполнена проверка двух отдельных процессов через HTTPS API с настоящим входом и CSRF-токеном. Дата проверки: " + result["checkedAt"] + ".")
summary = {"result": result["result"], "first": result["first"], "second": result["second"],
           "propertyId": result["createdOnFirst"]["id"],
           "rentCreatedOnFirstReadOnSecond": result["readOnSecond"]["monthlyRent"],
           "rentUpdatedOnSecondReadOnFirst": result["updatedOnSecondReadOnFirst"]["monthlyRent"],
           "deletedStatusOnSecond": result["deletedStatusOnSecond"],
           "auditActionsFirst": [e["action"] for e in result["auditFirst"]],
           "auditActionsSecond": [e["action"] for e in result["auditSecond"]]}
code(json.dumps(summary, ensure_ascii=False, indent=2))
paragraph("Объект, созданный на порту 8443, прочитан на порту 9443. Изменение цены до 65000 на втором экземпляре получено первым. После удаления через первый экземпляр второй вернул HTTP 404. В обоих журналах появились CREATE, UPDATE и DELETE. Полный протокол сохранён в verification/https-replication.json; проверку можно повторить скриптом tools/verify_replication.py.")
heading("Ограничения реализации:")
paragraph("Репликация асинхронна: успешный REST-ответ подтверждает локальную запись, но не завершение синхронизации всех экземпляров. В режиме репликации журнал аудита пополняется после обработки события. События в outbox повторяются при временной недоступности Kafka, пока работает процесс.")
paragraph("H2, outbox и отметки обработки находятся в памяти: аварийная остановка до доставки события может привести к его потере. Восстановление выполненных изменений зависит от сохранности и срока хранения журнала Kafka. Конфигурация использует один брокер и предназначена для учебной демонстрации; для промышленного применения нужны постоянная база, политика снимков состояния и отказоустойчивость брокера.")
heading("Вывод:")
paragraph("В лабораторной работе №7 реализована распределённая репликация данных недвижимости через Apache Kafka. Два экземпляра приложения работают на разных HTTPS-портах с независимыми H2 и обмениваются событиями создания, изменения и удаления. Добавлены защита от повторного применения событий, предотвращение зацикливания и восстановление из журнала. Работа подтверждена автоматическими тестами и проверкой через HTTPS API. Требования основного варианта задания выполнены.")
parts.append("</body></html>")
(root / "ОтчетЛР7Кочнев4431.html").write_text("\n".join(parts), encoding="utf-8")
print("Created ОтчетЛР7Кочнев4431.html")
