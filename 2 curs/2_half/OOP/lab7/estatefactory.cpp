#include "estatefactory.h"
#include "apartmentcalc.h"
#include "luxuriousapartmentcalc.h"
#include "townhousecalc.h"
#include "cottagecalc.h"

std::unique_ptr<AbstractCalc> ApartmentCalcFactory::create() const
{
    return std::make_unique<ApartmentCalc>();
}

std::unique_ptr<AbstractCalc> LuxuriousApartmentCalcFactory::create() const
{
    return std::make_unique<LuxuriousApartmentCalc>();
}

std::unique_ptr<AbstractCalc> TownhouseCalcFactory::create() const
{
    return std::make_unique<TownhouseCalc>();
}

std::unique_ptr<AbstractCalc> CottageCalcFactory::create() const
{
    return std::make_unique<CottageCalc>();
}
