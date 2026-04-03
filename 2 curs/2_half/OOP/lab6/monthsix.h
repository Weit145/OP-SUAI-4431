#ifndef MONTHSIX_H
#define MONTHSIX_H

#include <QObject>
#include <estate.h>

class MonthSix : public QObject
{
    Q_OBJECT
public:
    explicit MonthSix(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // MONTHSIX_H
