#ifndef COMMISSIONSTRATEGY_H
#define COMMISSIONSTRATEGY_H

// Поведенческий паттерн Strategy: разные алгоритмы расчёта комиссии
// вынесены в отдельные классы и могут заменяться без изменения фасада.
class CommissionStrategy
{
public:
    virtual ~CommissionStrategy() = default;
    virtual double calculate(double price, double realtorPercent) const = 0;
};

// Комиссия при продаже рассчитывается как процент от полной стоимости объекта.
class SaleCommissionStrategy final : public CommissionStrategy
{
public:
    double calculate(double price, double realtorPercent) const override;
};

// Комиссия при аренде рассчитывается от месячной ставки.
class RentCommissionStrategy final : public CommissionStrategy
{
public:
    double calculate(double price, double realtorPercent) const override;
};

#endif // COMMISSIONSTRATEGY_H
