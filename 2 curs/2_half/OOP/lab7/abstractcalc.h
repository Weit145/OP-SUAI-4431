#ifndef ABSTRACTCALC_H
#define ABSTRACTCALC_H

#include "estate.h"

// Абстрактный продукт паттерна Factory Method.
// Через этот интерфейс фасад обращается ко всем алгоритмам расчёта одинаково,
// не зная их конкретного типа.
class AbstractCalc
{
public:
    virtual ~AbstractCalc() = default;

    // Единый полиморфный интерфейс для получения стоимости или коэффициента.
    virtual double getCost(const Estate *value) const = 0;
};

#endif // ABSTRACTCALC_H
