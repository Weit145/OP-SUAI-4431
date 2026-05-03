#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include <QVariantMap>
#include "entities.h"

// Абстрактная фабрика для создания сущностей предметной области.
// Используется паттерн Factory Method: клиентский код работает с общим
// интерфейсом фабрики и не зависит от конкретного класса создаваемого объекта.
class EntityFactory
{
public:
    virtual ~EntityFactory() = default;
    virtual std::unique_ptr<Entity> create(const QVariantMap &data) const = 0;
};

class ClientFactory final : public EntityFactory
{
public:
    std::unique_ptr<Entity> create(const QVariantMap &data) const override;
};

class RealtorFactory final : public EntityFactory
{
public:
    std::unique_ptr<Entity> create(const QVariantMap &data) const override;
};

class PropertyFactory final : public EntityFactory
{
public:
    std::unique_ptr<Entity> create(const QVariantMap &data) const override;
};

class RequestFactory final : public EntityFactory
{
public:
    std::unique_ptr<Entity> create(const QVariantMap &data) const override;
};

class ShowingFactory final : public EntityFactory
{
public:
    std::unique_ptr<Entity> create(const QVariantMap &data) const override;
};

class DealFactory final : public EntityFactory
{
public:
    std::unique_ptr<Entity> create(const QVariantMap &data) const override;
};

#endif // ENTITYFACTORY_H
