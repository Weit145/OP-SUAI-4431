#include "apartmentcalc.h"

// Для квартиры эконом-класса используется коэффициент 100 у.е. за 1 м²
// на каждого проживающего.
double ApartmentCalc::getCost(const Estate *value) const
{
    return value->area * 100 * value->residents;
}
