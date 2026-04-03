#include "yearone.h"

YearOne::YearOne(QObject *parent)
    : QObject{parent}
{}

double YearOne::getCost(Estate *value){
    return 2;
}