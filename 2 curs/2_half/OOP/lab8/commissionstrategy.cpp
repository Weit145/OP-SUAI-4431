#include "commissionstrategy.h"

// Для продажи комиссия агентства равна проценту риелтора от цены сделки.
double SaleCommissionStrategy::calculate(double price, double realtorPercent) const
{
    return price * realtorPercent / 100.0;
}

// Для аренды агентство берёт половину месячной ставки плюс процент риелтора.
double RentCommissionStrategy::calculate(double price, double realtorPercent) const
{
    return price * 0.5 + price * realtorPercent / 100.0;
}
