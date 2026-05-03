#ifndef AGENCYFACADE_H
#define AGENCYFACADE_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QStringList>
#include "entities.h"

// Структурный паттерн Facade: класс скрывает коллекции сущностей,
// создание объектов через фабрики, поиск подходящих объектов и проведение сделок.
class AgencyFacade : public QObject
{
    Q_OBJECT
public:
    explicit AgencyFacade(QObject *parent = nullptr);
    ~AgencyFacade() override;

    Client *addClient(const QString &fullName, const QString &phone,
                      double budget, const QString &preferredDistrict);
    Realtor *addRealtor(const QString &fullName, const QString &phone,
                        const QString &specialization, double commissionPercent);
    Property *addProperty(const QString &address, const QString &district,
                          Property::Type type, int rooms, double area,
                          double price, Property::Status status);
    Request *addRequest(int clientId, Property::Type targetType,
                        const QString &district, int minRooms, double maxPrice);
    Showing *addShowing(int clientId, int propertyId, int realtorId,
                        const QDate &date, const QString &result);
    Deal *addDeal(int clientId, int propertyId, int realtorId,
                  const QDate &date, Deal::Operation operation, double finalPrice);

    QVector<QStringList> clientRows() const;
    QVector<QStringList> realtorRows() const;
    QVector<QStringList> propertyRows() const;
    QVector<QStringList> requestRows() const;
    QVector<QStringList> showingRows() const;
    QVector<QStringList> dealRows() const;

    QVector<QPair<int, QString>> clientOptions() const;
    QVector<QPair<int, QString>> realtorOptions() const;
    QVector<QPair<int, QString>> propertyOptions(bool onlyFree = false) const;
    QVector<QPair<int, QString>> requestOptions() const;

    QStringList suitablePropertiesForRequest(int requestId) const;

signals:
    // Сигнал является реализацией паттерна Observer: форма подписана на изменения
    // данных и обновляет таблицы, когда фасад сообщает о новом состоянии.
    void dataChanged();

private:
    void seedData();

    Client *findClient(int id) const;
    Realtor *findRealtor(int id) const;
    Property *findProperty(int id) const;
    Request *findRequest(int id) const;

    QVector<Client *> clients;
    QVector<Realtor *> realtors;
    QVector<Property *> properties;
    QVector<Request *> requests;
    QVector<Showing *> showings;
    QVector<Deal *> deals;

    int nextClientId = 1;
    int nextRealtorId = 1;
    int nextPropertyId = 1;
    int nextRequestId = 1;
    int nextShowingId = 1;
    int nextDealId = 1;
};

#endif // AGENCYFACADE_H
