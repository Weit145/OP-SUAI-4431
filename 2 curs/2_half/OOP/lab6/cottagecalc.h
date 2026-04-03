#ifndef COTTAGECALC_H
#define COTTAGECALC_H

#include <QObject>
#include <estate.h>

class CottageCalc : public QObject
{
    Q_OBJECT
public:
    explicit CottageCalc(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // COTTAGECALC_H
