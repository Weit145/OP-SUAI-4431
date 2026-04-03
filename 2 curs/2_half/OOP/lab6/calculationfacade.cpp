#include "calculationfacade.h"
#include "apartmentcalc.h"
#include "luxuriousapartmentcalc.h"
#include "townhousecalc.h"
#include "cottagecalc.h"
#include "monthsix.h"
#include "yearone.h"
#include "montheighteen.h"

calculationfacade::calculationfacade(QObject *parent)
    : QObject{parent}
{}

int calculationfacade::getCost(Estate *value)
{
    int cost;
    switch (value->getType()) {
    case Estate::EstateType::ECONOM:
        cost = ApartmentCalc::getCost(value);
        break;
    case Estate::EstateType::LUXURIOUS:
        cost = LuxuriousApartmentCalc::getCost(value);
        break;
    case Estate::EstateType::TOWN_HOUSE:
        cost = TownhouseCalc::getCost(value);
        break;
    case Estate::EstateType::COTTAGE:
        cost = CottageCalc::getCost(value);
        break;
    default:
        cost = -1;
        break;
    }

    switch (value->getTime()){
    case Estate::EstateMonths::months_6:
        cost*= MonthSix::getCost(value);
        break;
    case Estate::EstateMonths::year_1:
        cost*= YearOne::getCost(value);
        break;
    case Estate::EstateMonths::months_18:
        cost*=MonthEighteen::getCost(value);
        break;
    default:
        cost = -1;
        break;
    }

    return cost;
}