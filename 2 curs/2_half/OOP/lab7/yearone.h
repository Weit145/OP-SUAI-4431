#ifndef YEARONE_H
#define YEARONE_H

#include "abstractcalc.h"

// Коэффициент для договора на 1 год.
class YearOne final : public AbstractCalc
{
public:
    // Возвращает коэффициент для годового страхования.
    double getCost(const Estate *value) const override;
};

#endif // YEARONE_H
