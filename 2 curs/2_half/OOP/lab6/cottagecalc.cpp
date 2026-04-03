#include "cottagecalc.h"

CottageCalc::CottageCalc(QObject *parent)
    : QObject{parent}
{}

double CottageCalc::getCost(Estate *value){
    return value->area*180*value->residents;
}