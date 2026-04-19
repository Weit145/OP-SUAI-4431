#include "yearone.h"
#include <QtGlobal>

// Годовой договор использует коэффициент 2.0.
double YearOne::getCost(const Estate *value) const
{
    Q_UNUSED(value);
    return 2.0;
}
