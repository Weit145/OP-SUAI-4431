#include "agencyfacade.h"
#include "entityfactory.h"
#include "commissionstrategy.h"

#include <QDate>
#include <QtAlgorithms>
#include <memory>

AgencyFacade::AgencyFacade(QObject *parent)
    : QObject(parent)
{
    seedData();
}

AgencyFacade::~AgencyFacade()
{
    qDeleteAll(clients);
    qDeleteAll(realtors);
    qDeleteAll(properties);
    qDeleteAll(requests);
    qDeleteAll(showings);
    qDeleteAll(deals);
}

Client *AgencyFacade::addClient(const QString &fullName, const QString &phone,
                                double budget, const QString &preferredDistrict)
{
    ClientFactory factory;
    QVariantMap data;
    data["id"] = nextClientId++;
    data["fullName"] = fullName;
    data["phone"] = phone;
    data["budget"] = budget;
    data["preferredDistrict"] = preferredDistrict;

    std::unique_ptr<Entity> entity = factory.create(data);
    Client *client = static_cast<Client *>(entity.release());
    clients.append(client);
    emit dataChanged();
    return client;
}

Realtor *AgencyFacade::addRealtor(const QString &fullName, const QString &phone,
                                  const QString &specialization, double commissionPercent)
{
    RealtorFactory factory;
    QVariantMap data;
    data["id"] = nextRealtorId++;
    data["fullName"] = fullName;
    data["phone"] = phone;
    data["specialization"] = specialization;
    data["commissionPercent"] = commissionPercent;

    std::unique_ptr<Entity> entity = factory.create(data);
    Realtor *realtor = static_cast<Realtor *>(entity.release());
    realtors.append(realtor);
    emit dataChanged();
    return realtor;
}

Property *AgencyFacade::addProperty(const QString &address, const QString &district,
                                    Property::Type type, int rooms, double area,
                                    double price, Property::Status status)
{
    PropertyFactory factory;
    QVariantMap data;
    data["id"] = nextPropertyId++;
    data["address"] = address;
    data["district"] = district;
    data["type"] = static_cast<int>(type);
    data["rooms"] = rooms;
    data["area"] = area;
    data["price"] = price;
    data["status"] = static_cast<int>(status);

    std::unique_ptr<Entity> entity = factory.create(data);
    Property *property = static_cast<Property *>(entity.release());
    properties.append(property);
    emit dataChanged();
    return property;
}

Request *AgencyFacade::addRequest(int clientId, Property::Type targetType,
                                  const QString &district, int minRooms, double maxPrice)
{
    if (findClient(clientId) == nullptr) {
        return nullptr;
    }

    RequestFactory factory;
    QVariantMap data;
    data["id"] = nextRequestId++;
    data["clientId"] = clientId;
    data["targetType"] = static_cast<int>(targetType);
    data["district"] = district;
    data["minRooms"] = minRooms;
    data["maxPrice"] = maxPrice;
    data["active"] = true;

    std::unique_ptr<Entity> entity = factory.create(data);
    Request *request = static_cast<Request *>(entity.release());
    requests.append(request);
    emit dataChanged();
    return request;
}

Showing *AgencyFacade::addShowing(int clientId, int propertyId, int realtorId,
                                  const QDate &date, const QString &result)
{
    if (findClient(clientId) == nullptr || findProperty(propertyId) == nullptr ||
        findRealtor(realtorId) == nullptr) {
        return nullptr;
    }

    ShowingFactory factory;
    QVariantMap data;
    data["id"] = nextShowingId++;
    data["clientId"] = clientId;
    data["propertyId"] = propertyId;
    data["realtorId"] = realtorId;
    data["date"] = date;
    data["result"] = result;

    std::unique_ptr<Entity> entity = factory.create(data);
    Showing *showing = static_cast<Showing *>(entity.release());
    showings.append(showing);
    emit dataChanged();
    return showing;
}

Deal *AgencyFacade::addDeal(int clientId, int propertyId, int realtorId,
                            const QDate &date, Deal::Operation operation, double finalPrice)
{
    Client *client = findClient(clientId);
    Realtor *realtor = findRealtor(realtorId);
    Property *property = findProperty(propertyId);

    if (client == nullptr || realtor == nullptr || property == nullptr) {
        return nullptr;
    }

    std::unique_ptr<CommissionStrategy> strategy;
    if (operation == Deal::Sale) {
        strategy = std::make_unique<SaleCommissionStrategy>();
        property->status = Property::Sold;
    } else {
        strategy = std::make_unique<RentCommissionStrategy>();
        property->status = Property::Rented;
    }

    const double commission = strategy->calculate(finalPrice, realtor->commissionPercent);

    DealFactory factory;
    QVariantMap data;
    data["id"] = nextDealId++;
    data["clientId"] = client->id;
    data["propertyId"] = property->id;
    data["realtorId"] = realtor->id;
    data["date"] = date;
    data["operation"] = static_cast<int>(operation);
    data["finalPrice"] = finalPrice;
    data["commission"] = commission;
    data["status"] = "Закрыта";

    std::unique_ptr<Entity> entity = factory.create(data);
    Deal *deal = static_cast<Deal *>(entity.release());
    deals.append(deal);

    for (Request *request : requests) {
        if (request->clientId == clientId) {
            request->active = false;
        }
    }

    emit dataChanged();
    return deal;
}

QVector<QStringList> AgencyFacade::clientRows() const
{
    QVector<QStringList> rows;
    for (const Client *client : clients) {
        rows.append(client->toRow());
    }
    return rows;
}

QVector<QStringList> AgencyFacade::realtorRows() const
{
    QVector<QStringList> rows;
    for (const Realtor *realtor : realtors) {
        rows.append(realtor->toRow());
    }
    return rows;
}

QVector<QStringList> AgencyFacade::propertyRows() const
{
    QVector<QStringList> rows;
    for (const Property *property : properties) {
        rows.append(property->toRow());
    }
    return rows;
}

QVector<QStringList> AgencyFacade::requestRows() const
{
    QVector<QStringList> rows;
    for (const Request *request : requests) {
        rows.append(request->toRow());
    }
    return rows;
}

QVector<QStringList> AgencyFacade::showingRows() const
{
    QVector<QStringList> rows;
    for (const Showing *showing : showings) {
        rows.append(showing->toRow());
    }
    return rows;
}

QVector<QStringList> AgencyFacade::dealRows() const
{
    QVector<QStringList> rows;
    for (const Deal *deal : deals) {
        rows.append(deal->toRow());
    }
    return rows;
}

QVector<QPair<int, QString>> AgencyFacade::clientOptions() const
{
    QVector<QPair<int, QString>> options;
    for (const Client *client : clients) {
        options.append(qMakePair(client->id, QString::number(client->id) + " — " + client->fullName));
    }
    return options;
}

QVector<QPair<int, QString>> AgencyFacade::realtorOptions() const
{
    QVector<QPair<int, QString>> options;
    for (const Realtor *realtor : realtors) {
        options.append(qMakePair(realtor->id, QString::number(realtor->id) + " — " + realtor->fullName));
    }
    return options;
}

QVector<QPair<int, QString>> AgencyFacade::propertyOptions(bool onlyFree) const
{
    QVector<QPair<int, QString>> options;
    for (const Property *property : properties) {
        if (onlyFree && property->status != Property::Free) {
            continue;
        }
        const QString text = QString::number(property->id) + " — " +
                             Property::typeToString(property->type) + ", " +
                             property->district + ", " + property->address;
        options.append(qMakePair(property->id, text));
    }
    return options;
}

QVector<QPair<int, QString>> AgencyFacade::requestOptions() const
{
    QVector<QPair<int, QString>> options;
    for (const Request *request : requests) {
        const QString text = QString::number(request->id) + " — клиент " +
                             QString::number(request->clientId) + ", " +
                             Property::typeToString(request->targetType) + ", " +
                             request->district;
        options.append(qMakePair(request->id, text));
    }
    return options;
}

QStringList AgencyFacade::suitablePropertiesForRequest(int requestId) const
{
    QStringList result;
    const Request *request = findRequest(requestId);
    if (request == nullptr) {
        return result;
    }

    for (const Property *property : properties) {
        if (property->status != Property::Free) {
            continue;
        }
        if (property->type != request->targetType) {
            continue;
        }
        if (!request->district.isEmpty() &&
            property->district.compare(request->district, Qt::CaseInsensitive) != 0) {
            continue;
        }
        if (property->rooms < request->minRooms || property->price > request->maxPrice) {
            continue;
        }

        result.append(QString("№%1: %2, %3, %4 комн., %5 руб.")
                      .arg(property->id)
                      .arg(property->address)
                      .arg(property->district)
                      .arg(property->rooms)
                      .arg(property->price, 0, 'f', 2));
    }

    if (result.isEmpty()) {
        result.append("Подходящие объекты не найдены");
    }
    return result;
}

void AgencyFacade::seedData()
{
    addClient("Иванов Иван Иванович", "+7-911-100-10-10", 6500000, "Центр");
    addClient("Петрова Анна Сергеевна", "+7-921-222-33-44", 45000, "Северный");

    addRealtor("Смирнов Олег Петрович", "+7-911-333-44-55", "Продажа квартир", 3.0);
    addRealtor("Кузнецова Мария Игоревна", "+7-921-555-66-77", "Аренда жилья", 2.0);

    addProperty("ул. Ленина, 10", "Центр", Property::Apartment, 2, 54.0, 6200000, Property::Free);
    addProperty("ул. Садовая, 5", "Северный", Property::Apartment, 1, 34.0, 40000, Property::Free);
    addProperty("пр. Победы, 18", "Южный", Property::House, 4, 120.0, 11500000, Property::Free);

    addRequest(1, Property::Apartment, "Центр", 2, 6500000);
    addShowing(1, 1, 1, QDate::currentDate(), "Клиент заинтересован");
}

Client *AgencyFacade::findClient(int id) const
{
    for (Client *client : clients) {
        if (client->id == id) {
            return client;
        }
    }
    return nullptr;
}

Realtor *AgencyFacade::findRealtor(int id) const
{
    for (Realtor *realtor : realtors) {
        if (realtor->id == id) {
            return realtor;
        }
    }
    return nullptr;
}

Property *AgencyFacade::findProperty(int id) const
{
    for (Property *property : properties) {
        if (property->id == id) {
            return property;
        }
    }
    return nullptr;
}

Request *AgencyFacade::findRequest(int id) const
{
    for (Request *request : requests) {
        if (request->id == id) {
            return request;
        }
    }
    return nullptr;
}
