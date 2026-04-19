#ifndef COTTAGECALC_H
#define COTTAGECALC_H

#include "abstractcalc.h"

// Расчёт для коттеджа.
class CottageCalc final : public AbstractCalc
{
public:
    // Возвращает базовую стоимость страхования для коттеджа.
    double getCost(const Estate *value) const override;
};

#endif // COTTAGECALC_H
