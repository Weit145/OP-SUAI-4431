#include "states.h"

States::States(QObject *parent)
    : QObject(parent), actualData(nullptr)
{
}

States::~States()
{
    // Владельцем всех сохранённых состояний является контейнер array.
    qDeleteAll(array);
    array.clear();
    actualData = nullptr;
}

bool States::hasStates() const
{
    return !array.isEmpty();
}

Estate *States::getActualData() const
{
    return actualData;
}

void States::add(Estate *value)
{
    array.append(value);
    actualData = value;
    emit notifyObservers();
}

void States::undo()
{
    if (array.isEmpty()) {
        actualData = nullptr;
        emit notifyObservers();
        return;
    }

    // takeLast извлекает последнее состояние из истории,
    // после чего его можно безопасно удалить.
    Estate *removedValue = array.takeLast();
    delete removedValue;

    actualData = array.isEmpty() ? nullptr : array.back();
    emit notifyObservers();
}
