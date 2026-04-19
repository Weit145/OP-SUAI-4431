#include "monthsix.h"
#include <QtGlobal>

// Аргумент value здесь не используется, но сигнатура метода остаётся общей
// для всех продуктов паттерна.
double MonthSix::getCost(const Estate *value) const
{
    Q_UNUSED(value);
    return 1.8;
}
