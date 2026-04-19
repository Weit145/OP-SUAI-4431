#ifndef ESTATEFACTORY_H
#define ESTATEFACTORY_H

#include "calcfactory.h"

// Конкретная фабрика для создания расчёта квартиры эконом-класса.
class ApartmentCalcFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

// Конкретная фабрика для создания расчёта элитной квартиры.
class LuxuriousApartmentCalcFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

// Конкретная фабрика для создания расчёта таун-хауса.
class TownhouseCalcFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

// Конкретная фабрика для создания расчёта коттеджа.
class CottageCalcFactory final : public CalcFactory
{
public:
    std::unique_ptr<AbstractCalc> create() const override;
};

#endif // ESTATEFACTORY_H
