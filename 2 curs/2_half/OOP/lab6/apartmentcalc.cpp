#include "apartmentcalc.h"

ApartmentCalc::ApartmentCalc(QObject *parent)
    : QObject{parent}
{}

double ApartmentCalc::getCost(Estate *value){
    return value->area*100*value->residents;
}