#include "estate.h"

Estate::Estate(QObject *parent)
    : QObject(parent)
{
    // Значения по умолчанию нужны для инициализации объекта до заполнения формы.
    age = 21;
    area = 100;
    residents = 1;
    owner = "Dima";
    type = ECONOM;
    months = months_18;
}

Estate::EstateType Estate::getType() const
{
    return type;
}

Estate::EstateMonths Estate::getTime() const
{
    return months;
}
