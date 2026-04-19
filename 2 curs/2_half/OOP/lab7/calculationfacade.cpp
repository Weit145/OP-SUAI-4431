#include "calculationfacade.h"
#include "estatefactory.h"
#include "periodfactory.h"

CalculationFacade::CalculationFacade(QObject *parent)
    : QObject(parent)
{
}

int CalculationFacade::getCost(const Estate *value)
{
    if (value == nullptr) {
        return -1;
    }

    // Фасад выбирает две фабрики: одну для базового тарифа по типу жилья,
    // вторую для коэффициента по сроку страхования.
    auto estateFactory = createEstateFactory(value->getType());
    auto periodFactory = createPeriodFactory(value->getTime());

    if (!estateFactory || !periodFactory) {
        return -1;
    }

    // Каждая фабрика создаёт конкретный объект расчёта через единый интерфейс.
    auto estateCalc = estateFactory->create();
    auto periodCalc = periodFactory->create();

    const double baseCost = estateCalc->getCost(value);
    const double periodCoefficient = periodCalc->getCost(value);

    return static_cast<int>(baseCost * periodCoefficient);
}

std::unique_ptr<CalcFactory> CalculationFacade::createEstateFactory(Estate::EstateType type)
{
    switch (type) {
    case Estate::ECONOM:
        return std::make_unique<ApartmentCalcFactory>();
    case Estate::LUXURIOUS:
        return std::make_unique<LuxuriousApartmentCalcFactory>();
    case Estate::TOWN_HOUSE:
        return std::make_unique<TownhouseCalcFactory>();
    case Estate::COTTAGE:
        return std::make_unique<CottageCalcFactory>();
    default:
        return nullptr;
    }
}

std::unique_ptr<CalcFactory> CalculationFacade::createPeriodFactory(Estate::EstateMonths months)
{
    switch (months) {
    case Estate::months_6:
        return std::make_unique<MonthSixFactory>();
    case Estate::year_1:
        return std::make_unique<YearOneFactory>();
    case Estate::months_18:
        return std::make_unique<MonthEighteenFactory>();
    default:
        return nullptr;
    }
}
