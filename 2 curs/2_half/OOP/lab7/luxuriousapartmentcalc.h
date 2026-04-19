#ifndef LUXURIOUSAPARTMENTCALC_H
#define LUXURIOUSAPARTMENTCALC_H

#include "abstractcalc.h"

// Расчёт для элитной квартиры.
class LuxuriousApartmentCalc final : public AbstractCalc
{
public:
    // Возвращает базовую стоимость страхования для элитного жилья.
    double getCost(const Estate *value) const override;
};

#endif // LUXURIOUSAPARTMENTCALC_H
