#ifndef CALCFACTORY_H
#define CALCFACTORY_H

#include <memory>
#include "abstractcalc.h"

// Абстрактная фабрика. Каждый конкретный наследник создаёт объект
// определённого класса расчёта, но фасад работает только с этим интерфейсом.
class CalcFactory
{
public:
    virtual ~CalcFactory() = default;

    // Фабричный метод создаёт конкретный объект расчёта.
    virtual std::unique_ptr<AbstractCalc> create() const = 0;
};

#endif // CALCFACTORY_H
