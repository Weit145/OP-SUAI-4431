#ifndef LUXURIOUSAPARTMENTCALC_H
#define LUXURIOUSAPARTMENTCALC_H

#include <QObject>
#include <estate.h>

class LuxuriousApartmentCalc : public QObject
{
    Q_OBJECT
public:
    explicit LuxuriousApartmentCalc(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // LUXURIOUSAPARTMENTCALC_H
