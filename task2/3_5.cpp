///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////////////////////

// Базовый интерфейс 1
class Entity_v1
{
public:
    virtual ~Entity_v1() = default;
    virtual void test() = 0; // выполняет одну задачу
};

// Базовый интерфейс 2
class Entity_v2
{
public:
    virtual ~Entity_v2() = default;
    virtual void test() = 0; // выполняет другую задачу
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Адаптер между Entity_v1 и Client
class Adapter_v1 : public Entity_v1
{
public:
    void test() override final
    {
        test_v1(); // перенаправляем вызов в уникальное имя
    }

protected:
    virtual void test_v1() = 0;
};

// Адаптер между Entity_v2 и Client
class Adapter_v2 : public Entity_v2
{
public:
    void test() override final
    {
        test_v2(); // перенаправляем вызов в уникальное имя
    }

protected:
    virtual void test_v2() = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Класс, наследующий обе линии через адаптеры
class Client : public Adapter_v1, public Adapter_v2
{
protected:
    void test_v1() override
    {
        std::cout << "Client::test_v1() - logic for Entity_v1\n";
        m_state = 1;
    }

    void test_v2() override
    {
        std::cout << "Client::test_v2() - logic for Entity_v2\n";
        m_state = 2;
    }

private:
    int m_state = 0;

public:
    int state() const { return m_state; }
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Client client;

    // через указатель на Entity_v1
    Entity_v1* e1 = &client;
    e1->test();
    assert(client.state() == 1);

    // через указатель на Entity_v2
    Entity_v2* e2 = &client;
    e2->test();
    assert(client.state() == 2);

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
