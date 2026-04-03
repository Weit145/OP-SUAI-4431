#include "luxuriousapartmentcalc.h"

LuxuriousApartmentCalc::LuxuriousApartmentCalc(QObject *parent)
    : QObject{parent}
{}

double LuxuriousApartmentCalc::getCost(Estate *value){
    return value->area*130*value->residents;
}