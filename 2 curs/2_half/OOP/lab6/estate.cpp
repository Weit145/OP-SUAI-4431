#include "estate.h"

Estate::Estate(QObject *parent)
    : QObject{parent}
{
    age = 21;
    area= 100;
    residents = 15;
    owner = "Dima";
    type = ECONOM;
    months = months_18;
}

Estate::EstateType Estate::getType()const{
    return this->type;
}

Estate::EstateMonths Estate::getTime()const{
    return this->months;
}