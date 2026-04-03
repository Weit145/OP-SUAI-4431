#include "townhousecalc.h"

TownhouseCalc::TownhouseCalc(QObject *parent)
    : QObject{parent}
{}

double TownhouseCalc::getCost(Estate *value){
    return value->area*150*value->residents;
}