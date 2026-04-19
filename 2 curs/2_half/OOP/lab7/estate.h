#ifndef ESTATE_H
#define ESTATE_H

#include <QObject>

// Класс хранит все входные данные, введённые пользователем на форме.
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

    enum EstateMonths {
        months_6,
        year_1,
        months_18,
    };

    explicit Estate(QObject *parent = nullptr);

    // Геттеры используются фасадом для выбора нужных фабрик.
    EstateType getType() const;
    EstateMonths getTime() const;

    int age;
    int area;
    int residents;
    EstateMonths months;
    EstateType type;
    QString owner;
};

#endif // ESTATE_H
