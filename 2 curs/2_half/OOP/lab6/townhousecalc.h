#ifndef TOWNHOUSECALC_H
#define TOWNHOUSECALC_H

#include <QObject>
#include <estate.h>

class TownhouseCalc : public QObject
{
    Q_OBJECT
public:
    explicit TownhouseCalc(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // TOWNHOUSECALC_H
