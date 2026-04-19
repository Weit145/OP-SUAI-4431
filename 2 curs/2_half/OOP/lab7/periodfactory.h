#ifndef PERIODFACTORY_H
#define PERIODFACTORY_H

#include "calcfactory.h"

// Конкретная фабрика для создания коэффициента срока 6 месяцев.
class MonthSixFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

// Конкретная фабрика для создания коэффициента срока 1 год.
class YearOneFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

// Конкретная фабрика для создания коэффициента срока 18 месяцев.
class MonthEighteenFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

#endif // PERIODFACTORY_H
