#include "entityfactory.h"

std::unique_ptr<Entity> ClientFactory::create(const QVariantMap &data) const
{
    return std::make_unique<Client>(
        data.value("id").toInt(),
        data.value("fullName").toString(),
        data.value("phone").toString(),
        data.value("budget").toDouble(),
        data.value("preferredDistrict").toString()
    );
}

std::unique_ptr<Entity> RealtorFactory::create(const QVariantMap &data) const
{
    return std::make_unique<Realtor>(
        data.value("id").toInt(),
        data.value("fullName").toString(),
        data.value("phone").toString(),
        data.value("specialization").toString(),
        data.value("commissionPercent").toDouble()
    );
}

std::unique_ptr<Entity> PropertyFactory::create(const QVariantMap &data) const
{
    return std::make_unique<Property>(
        data.value("id").toInt(),
        data.value("address").toString(),
        data.value("district").toString(),
        static_cast<Property::Type>(data.value("type").toInt()),
        data.value("rooms").toInt(),
        data.value("area").toDouble(),
        data.value("price").toDouble(),
        static_cast<Property::Status>(data.value("status").toInt())
    );
}

std::unique_ptr<Entity> RequestFactory::create(const QVariantMap &data) const
{
    return std::make_unique<Request>(
        data.value("id").toInt(),
        data.value("clientId").toInt(),
        static_cast<Property::Type>(data.value("targetType").toInt()),
        data.value("district").toString(),
        data.value("minRooms").toInt(),
        data.value("maxPrice").toDouble(),
        data.value("active").toBool()
    );
}

std::unique_ptr<Entity> ShowingFactory::create(const QVariantMap &data) const
{
    return std::make_unique<Showing>(
        data.value("id").toInt(),
        data.value("clientId").toInt(),
        data.value("propertyId").toInt(),
        data.value("realtorId").toInt(),
        data.value("date").toDate(),
        data.value("result").toString()
    );
}

std::unique_ptr<Entity> DealFactory::create(const QVariantMap &data) const
{
    return std::make_unique<Deal>(
        data.value("id").toInt(),
        data.value("clientId").toInt(),
        data.value("propertyId").toInt(),
        data.value("realtorId").toInt(),
        data.value("date").toDate(),
        static_cast<Deal::Operation>(data.value("operation").toInt()),
        data.value("finalPrice").toDouble(),
        data.value("commission").toDouble(),
        data.value("status").toString()
    );
}
