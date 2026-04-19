#ifndef APARTMENTCALC_H
#define APARTMENTCALC_H

#include "abstractcalc.h"

// Расчёт для квартиры эконом-класса.
class ApartmentCalc final : public AbstractCalc
{
public:
    // Возвращает базовую стоимость страхования для выбранного типа жилья.
    double getCost(const Estate *value) const override;
};

#endif // APARTMENTCALC_H
