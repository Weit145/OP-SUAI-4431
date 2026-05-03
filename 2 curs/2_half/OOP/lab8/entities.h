#ifndef ENTITIES_H
#define ENTITIES_H

#include <QString>
#include <QStringList>
#include <QDate>

// Базовая сущность предметной области. Нужна для фабрик и единого вывода в таблицы.
class Entity
{
public:
    virtual ~Entity() = default;

    // Возвращает программное имя сущности.
    virtual QString entityName() const = 0;

    // Возвращает строку для отображения объекта в таблице.
    virtual QStringList toRow() const = 0;
};

// Сущность "Клиент" хранит информацию о покупателе или арендаторе.
class Client final : public Entity
{
public:
    Client(int id, const QString &fullName, const QString &phone,
           double budget, const QString &preferredDistrict);

    QString entityName() const override;
    QStringList toRow() const override;

    int id;                      // Уникальный идентификатор клиента.
    QString fullName;             // ФИО клиента.
    QString phone;                // Контактный телефон клиента.
    double budget;                // Максимальный бюджет клиента.
    QString preferredDistrict;    // Предпочитаемый район поиска.
};

// Сущность "Риелтор" хранит данные сотрудника агентства.
class Realtor final : public Entity
{
public:
    Realtor(int id, const QString &fullName, const QString &phone,
            const QString &specialization, double commissionPercent);

    QString entityName() const override;
    QStringList toRow() const override;

    int id;                       // Уникальный идентификатор риелтора.
    QString fullName;             // ФИО риелтора.
    QString phone;                // Телефон для связи.
    QString specialization;       // Специализация: продажа, аренда, элитная недвижимость.
    double commissionPercent;     // Процент комиссии риелтора.
};

// Сущность "Объект недвижимости" хранит описание квартиры, дома или помещения.
class Property final : public Entity
{
public:
    enum Type {
        Apartment,
        House,
        Commercial
    };

    enum Status {
        Free,
        Reserved,
        Sold,
        Rented
    };

    Property(int id, const QString &address, const QString &district,
             Type type, int rooms, double area, double price, Status status);

    QString entityName() const override;
    QStringList toRow() const override;

    static QString typeToString(Type type);
    static QString statusToString(Status status);

    int id;                       // Уникальный идентификатор объекта недвижимости.
    QString address;              // Адрес объекта.
    QString district;             // Район города.
    Type type;                    // Тип объекта: квартира, дом, коммерческое помещение.
    int rooms;                    // Количество комнат.
    double area;                  // Площадь объекта в квадратных метрах.
    double price;                 // Цена продажи или месячная арендная ставка.
    Status status;                // Состояние объекта: свободен, бронь, продан, сдан.
};

// Сущность "Заявка" описывает требования клиента к объекту недвижимости.
class Request final : public Entity
{
public:
    Request(int id, int clientId, Property::Type targetType,
            const QString &district, int minRooms, double maxPrice, bool active);

    QString entityName() const override;
    QStringList toRow() const override;

    int id;                       // Уникальный идентификатор заявки.
    int clientId;                 // Идентификатор клиента, которому принадлежит заявка.
    Property::Type targetType;    // Желаемый тип недвижимости.
    QString district;             // Желаемый район.
    int minRooms;                 // Минимальное количество комнат.
    double maxPrice;              // Максимальная цена.
    bool active;                  // Признак активности заявки.
};

// Сущность "Показ" фиксирует просмотр объекта клиентом.
class Showing final : public Entity
{
public:
    Showing(int id, int clientId, int propertyId, int realtorId,
            const QDate &date, const QString &result);

    QString entityName() const override;
    QStringList toRow() const override;

    int id;                       // Уникальный идентификатор показа.
    int clientId;                 // Клиент, который смотрит объект.
    int propertyId;               // Объект недвижимости, который показывают.
    int realtorId;                // Риелтор, проводящий показ.
    QDate date;                   // Дата показа.
    QString result;               // Результат: заинтересован, отказ, думает.
};

// Сущность "Сделка" связывает клиента, объект недвижимости и риелтора.
class Deal final : public Entity
{
public:
    enum Operation {
        Sale,
        Rent
    };

    Deal(int id, int clientId, int propertyId, int realtorId,
         const QDate &date, Operation operation, double finalPrice,
         double commission, const QString &status);

    QString entityName() const override;
    QStringList toRow() const override;

    static QString operationToString(Operation operation);

    int id;                       // Уникальный идентификатор сделки.
    int clientId;                 // Клиент, заключивший сделку.
    int propertyId;               // Объект сделки.
    int realtorId;                // Ответственный риелтор.
    QDate date;                   // Дата заключения сделки.
    Operation operation;          // Тип операции: продажа или аренда.
    double finalPrice;            // Итоговая стоимость сделки.
    double commission;            // Рассчитанная комиссия агентства.
    QString status;               // Статус сделки.
};

#endif // ENTITIES_H
