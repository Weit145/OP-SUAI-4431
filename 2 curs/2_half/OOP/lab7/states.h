#ifndef STATES_H
#define STATES_H

#include <QObject>
#include <QList>
#include "estate.h"

// Класс States хранит историю введённых пользователем запросов.
// Благодаря этому можно вернуться к предыдущему состоянию формы.
class States : public QObject
{
    Q_OBJECT
public:
    explicit States(QObject *parent = nullptr);
    ~States();

    // Удаляет последнее состояние и делает актуальным предыдущее.
    void undo();

    // Проверяет, есть ли в истории хотя бы один сохранённый запрос.
    bool hasStates() const;

    // Возвращает текущее состояние формы.
    Estate *getActualData() const;

    // Добавляет новый снимок данных в историю.
    void add(Estate *value);

signals:
    // Сигнал сообщает форме о том, что история изменилась.
    void notifyObservers();

private:
    QList<Estate *> array;
    Estate *actualData;
};

#endif // STATES_H
