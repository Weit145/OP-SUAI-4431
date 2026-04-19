#ifndef TOWNHOUSECALC_H
#define TOWNHOUSECALC_H

#include "abstractcalc.h"

// Расчёт для таун-хауса.
class TownhouseCalc final : public AbstractCalc
{
public:
    // Возвращает базовую стоимость страхования для таун-хауса.
    double getCost(const Estate *value) const override;
};

#endif // TOWNHOUSECALC_H
