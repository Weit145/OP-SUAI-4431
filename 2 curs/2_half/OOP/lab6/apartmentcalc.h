#ifndef APARTMENTCALC_H
#define APARTMENTCALC_H

#include <QObject>
#include <estate.h>

class ApartmentCalc : public QObject
{
    Q_OBJECT
public:
    explicit ApartmentCalc(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // APARTMENTCALC_H
