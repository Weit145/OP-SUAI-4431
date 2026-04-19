#ifndef MONTHSIX_H
#define MONTHSIX_H

#include "abstractcalc.h"

// Коэффициент для договора на 6 месяцев.
class MonthSix final : public AbstractCalc
{
public:
    // Возвращает коэффициент для выбранного срока страхования.
    double getCost(const Estate *value) const override;
};

#endif // MONTHSIX_H
