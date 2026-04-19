#ifndef MONTHEIGHTEEN_H
#define MONTHEIGHTEEN_H

#include "abstractcalc.h"

// Коэффициент для договора на 18 месяцев.
class MonthEighteen final : public AbstractCalc
{
public:
    // Возвращает коэффициент для длительного страхования.
    double getCost(const Estate *value) const override;
};

#endif // MONTHEIGHTEEN_H
