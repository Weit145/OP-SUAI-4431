#include "periodfactory.h"
#include "monthsix.h"
#include "yearone.h"
#include "montheighteen.h"

std::unique_ptr<AbstractCalc> MonthSixFactory::create() const
{
    return std::make_unique<MonthSix>();
}

std::unique_ptr<AbstractCalc> YearOneFactory::create() const
{
    return std::make_unique<YearOne>();
}

std::unique_ptr<AbstractCalc> MonthEighteenFactory::create() const
{
    return std::make_unique<MonthEighteen>();
}
