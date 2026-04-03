#ifndef YEARONE_H
#define YEARONE_H

#include <QObject>
#include <estate.h>

class YearOne : public QObject
{
    Q_OBJECT
public:
    explicit YearOne(QObject *parent = nullptr);
    static double getCost(Estate *value);
signals:
};

#endif // YEARONE_H
