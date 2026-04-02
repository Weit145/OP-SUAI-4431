#ifndef ESTATE_H
#define ESTATE_H

#include <QObject>

class Estate : public QObject
{
    Q_OBJECT
public:
    enum EstateType {
        ECONOM,
        LUXURIOUS,
        TOWN_HOUSE,
        COTTAGE
    };
    enum EstateMonths{
        months_6,
        year_1,
        months_18,
    };

    explicit Estate(QObject *parent = nullptr);
    EstateType getType() const;

signals:
private:
    int age;
    int area;
    int residents;
    EstateMonths months;
    EstateType type;
    QString owner;
};

#endif // ESTATE_H
