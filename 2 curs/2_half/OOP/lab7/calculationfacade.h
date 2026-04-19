#ifndef CALCULATIONFACADE_H
#define CALCULATIONFACADE_H

#include <memory>
#include <QObject>
#include "calcfactory.h"
#include "estate.h"

// Фасад инкапсулирует общую последовательность вычисления стоимости.
// Клиентский код не знает, какие именно классы будут созданы для конкретного
// типа жилья и срока страхования.
class CalculationFacade : public QObject
{
    Q_OBJECT
public:
    explicit CalculationFacade(QObject *parent = nullptr);

    // Главный метод фасада: выбирает нужные фабрики, создаёт продукты
    // и возвращает итоговую стоимость страхового взноса.
    static int getCost(const Estate *value);

private:
    // Создание фабрики для выбора тарифа по типу жилья.
    static std::unique_ptr<CalcFactory> createEstateFactory(Estate::EstateType type);

    // Создание фабрики для выбора коэффициента по сроку страхования.
    static std::unique_ptr<CalcFactory> createPeriodFactory(Estate::EstateMonths months);
};

#endif // CALCULATIONFACADE_H
