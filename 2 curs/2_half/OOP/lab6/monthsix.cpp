#include "monthsix.h"

MonthSix::MonthSix(QObject *parent)
    : QObject{parent}
{}

double MonthSix::getCost(Estate *value){
    return 1.8;
}