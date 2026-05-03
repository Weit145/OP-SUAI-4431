#include "entities.h"

Client::Client(int id, const QString &fullName, const QString &phone,
               double budget, const QString &preferredDistrict)
    : id(id), fullName(fullName), phone(phone), budget(budget),
      preferredDistrict(preferredDistrict)
{
}

QString Client::entityName() const
{
    return "Client";
}

QStringList Client::toRow() const
{
    return {QString::number(id), fullName, phone,
            QString::number(budget, 'f', 2), preferredDistrict};
}

Realtor::Realtor(int id, const QString &fullName, const QString &phone,
                 const QString &specialization, double commissionPercent)
    : id(id), fullName(fullName), phone(phone), specialization(specialization),
      commissionPercent(commissionPercent)
{
}

QString Realtor::entityName() const
{
    return "Realtor";
}

QStringList Realtor::toRow() const
{
    return {QString::number(id), fullName, phone, specialization,
            QString::number(commissionPercent, 'f', 2)};
}

Property::Property(int id, const QString &address, const QString &district,
                   Type type, int rooms, double area, double price, Status status)
    : id(id), address(address), district(district), type(type), rooms(rooms),
      area(area), price(price), status(status)
{
}

QString Property::entityName() const
{
    return "Property";
}

QStringList Property::toRow() const
{
    return {QString::number(id), address, district, typeToString(type),
            QString::number(rooms), QString::number(area, 'f', 1),
            QString::number(price, 'f', 2), statusToString(status)};
}

QString Property::typeToString(Property::Type type)
{
    switch (type) {
    case Apartment:
        return "Квартира";
    case House:
        return "Дом";
    case Commercial:
        return "Коммерческая";
    }
    return "Неизвестно";
}

QString Property::statusToString(Property::Status status)
{
    switch (status) {
    case Free:
        return "Свободен";
    case Reserved:
        return "Забронирован";
    case Sold:
        return "Продан";
    case Rented:
        return "Сдан";
    }
    return "Неизвестно";
}

Request::Request(int id, int clientId, Property::Type targetType,
                 const QString &district, int minRooms, double maxPrice, bool active)
    : id(id), clientId(clientId), targetType(targetType), district(district),
      minRooms(minRooms), maxPrice(maxPrice), active(active)
{
}

QString Request::entityName() const
{
    return "Request";
}

QStringList Request::toRow() const
{
    return {QString::number(id), QString::number(clientId),
            Property::typeToString(targetType), district,
            QString::number(minRooms), QString::number(maxPrice, 'f', 2),
            active ? "Активна" : "Закрыта"};
}

Showing::Showing(int id, int clientId, int propertyId, int realtorId,
                 const QDate &date, const QString &result)
    : id(id), clientId(clientId), propertyId(propertyId), realtorId(realtorId),
      date(date), result(result)
{
}

QString Showing::entityName() const
{
    return "Showing";
}

QStringList Showing::toRow() const
{
    return {QString::number(id), QString::number(clientId),
            QString::number(propertyId), QString::number(realtorId),
            date.toString("dd.MM.yyyy"), result};
}

Deal::Deal(int id, int clientId, int propertyId, int realtorId,
           const QDate &date, Operation operation, double finalPrice,
           double commission, const QString &status)
    : id(id), clientId(clientId), propertyId(propertyId), realtorId(realtorId),
      date(date), operation(operation), finalPrice(finalPrice),
      commission(commission), status(status)
{
}

QString Deal::entityName() const
{
    return "Deal";
}

QStringList Deal::toRow() const
{
    return {QString::number(id), QString::number(clientId),
            QString::number(propertyId), QString::number(realtorId),
            date.toString("dd.MM.yyyy"), operationToString(operation),
            QString::number(finalPrice, 'f', 2), QString::number(commission, 'f', 2),
            status};
}

QString Deal::operationToString(Deal::Operation operation)
{
    switch (operation) {
    case Sale:
        return "Продажа";
    case Rent:
        return "Аренда";
    }
    return "Неизвестно";
}
