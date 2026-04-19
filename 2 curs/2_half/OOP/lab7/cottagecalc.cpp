#include "cottagecalc.h"

// Для коттеджа используется максимальный тариф среди всех типов жилья.
double CottageCalc::getCost(const Estate *value) const
{
    return value->area * 180 * value->residents;
}
