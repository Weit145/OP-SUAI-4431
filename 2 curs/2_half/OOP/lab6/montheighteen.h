#ifndef MONTHEIGHTEEN_H
#define MONTHEIGHTEEN_H

#include <QObject>
#include <estate.h>

class MonthEighteen : public QObject
{
    Q_OBJECT
public:
    explicit MonthEighteen(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // MONTHEIGHTEEN_H
