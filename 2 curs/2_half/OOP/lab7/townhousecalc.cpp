#include "townhousecalc.h"

// Для таун-хауса применяется тариф 150 у.е. за 1 м²
// на каждого проживающего.
double TownhouseCalc::getCost(const Estate *value) const
{
    return value->area * 150 * value->residents;
}
