#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace builder_example
{
struct Entity
{
    int x = 0;
    int y = 0;
};

class Builder
{
public:
    virtual ~Builder() = default;

    std::unique_ptr<Entity> make_entity()
    {
        auto entity = std::make_unique<Entity>();
        set_x(*entity);
        set_y(*entity);
        return entity;
    }

    virtual void set_x(Entity& entity) const = 0;
    virtual void set_y(Entity& entity) const = 0;
};

class ClientBuilder final : public Builder
{
public:
    void set_x(Entity& entity) const override
    {
        entity.x = 1;
    }

    void set_y(Entity& entity) const override
    {
        entity.y = 2;
    }
};
}

namespace factory_example
{
class Entity
{
public:
    virtual ~Entity() = default;
    virtual std::string kind() const = 0;
};

class Client final : public Entity
{
public:
    std::string kind() const override
    {
        return "client";
    }
};

class Factory
{
public:
    virtual ~Factory() = default;
    virtual std::unique_ptr<Entity> make_entity() const = 0;
};

class ClientFactory final : public Factory
{
public:
    std::unique_ptr<Entity> make_entity() const override
    {
        return std::make_unique<Client>();
    }
};
}

namespace prototype_example
{
class Entity
{
public:
    virtual ~Entity() = default;
    virtual std::unique_ptr<Entity> copy() const = 0;
    virtual std::string kind() const = 0;
};

class Client final : public Entity
{
public:
    std::unique_ptr<Entity> copy() const override
    {
        return std::make_unique<Client>(*this);
    }

    std::string kind() const override
    {
        return "client";
    }
};

class Prototype
{
public:
    Prototype()
    {
        m_entities.push_back(std::make_unique<Client>());
    }

    std::unique_ptr<Entity> make_client() const
    {
        return m_entities.front()->copy();
    }

private:
    std::vector<std::unique_ptr<Entity>> m_entities;
};
}

namespace composite_example
{
class Entity
{
public:
    virtual ~Entity() = default;
    virtual int test() const = 0;
};

class Client final : public Entity
{
public:
    int test() const override
    {
        return 1;
    }
};

class Server final : public Entity
{
public:
    int test() const override
    {
        return 2;
    }
};

class Composite final : public Entity
{
public:
    void add(std::unique_ptr<Entity> entity)
    {
        m_entities.push_back(std::move(entity));
    }

    int test() const override
    {
        int sum = 0;
        for (auto const& entity : m_entities)
        {
            sum += entity->test();
        }
        return sum;
    }

private:
    std::vector<std::unique_ptr<Entity>> m_entities;
};
}

namespace observer_example
{
class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(int value) = 0;
};

class Entity
{
public:
    void add(std::shared_ptr<Observer> observer)
    {
        m_observers.push_back(std::move(observer));
    }

    void set(int value)
    {
        m_value = value;
        for (auto const& observer : m_observers)
        {
            observer->update(m_value);
        }
    }

private:
    int m_value = 0;
    std::vector<std::shared_ptr<Observer>> m_observers;
};

class Client final : public Observer
{
public:
    void update(int value) override
    {
        m_last_value = value;
    }

    int last_value() const
    {
        return m_last_value;
    }

private:
    int m_last_value = 0;
};
}

int main()
{
    builder_example::ClientBuilder builder;
    std::unique_ptr<builder_example::Entity> built = builder.make_entity();
    if (built->x != 1 || built->y != 2)
    {
        std::cerr << "Builder example failed\n";
        return 1;
    }

    factory_example::ClientFactory factory;
    std::unique_ptr<factory_example::Entity> made = factory.make_entity();
    if (made->kind() != "client")
    {
        std::cerr << "Factory example failed\n";
        return 2;
    }

    prototype_example::Prototype prototype;
    std::unique_ptr<prototype_example::Entity> clone = prototype.make_client();
    if (clone->kind() != "client")
    {
        std::cerr << "Prototype example failed\n";
        return 3;
    }

    composite_example::Composite composite;
    composite.add(std::make_unique<composite_example::Client>());
    composite.add(std::make_unique<composite_example::Server>());
    if (composite.test() != 3)
    {
        std::cerr << "Composite example failed\n";
        return 4;
    }

    observer_example::Entity entity;
    auto observer = std::make_shared<observer_example::Client>();
    entity.add(observer);
    entity.set(42);
    if (observer->last_value() != 42)
    {
        std::cerr << "Observer example failed\n";
        return 5;
    }

    std::cout << "09.03: smart pointer examples completed\n";
    return 0;
}
