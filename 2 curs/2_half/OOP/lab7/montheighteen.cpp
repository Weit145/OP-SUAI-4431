#include "montheighteen.h"
#include <QtGlobal>

// Самый длинный срок страхования даёт коэффициент 2.8.
double MonthEighteen::getCost(const Estate *value) const
{
    Q_UNUSED(value);
    return 2.8;
}
