#include "luxuriousapartmentcalc.h"

// Для элитной квартиры используется повышенный тариф.
double LuxuriousApartmentCalc::getCost(const Estate *value) const
{
    return value->area * 130 * value->residents;
}
