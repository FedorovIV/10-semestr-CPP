///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////

class Entity
{
private:
    // приватные функции, которые нужно протестировать
    void test_v1()
    {
        std::cout << "Entity::test_v1() called\n";
        m_value = 42;
    }

    void test_v2()
    {
        std::cout << "Entity::test_v2() called\n";
        m_value *= 2;
    }

    int m_value = 0;

    // --------------- паттерн "Attorney–Client" ----------------
    friend class Tester_v1;
    friend class Tester_v2;

public:
    int value() const { return m_value; }
};

///////////////////////////////////////////////////////////////////////////////////////////////

// "Адвокат" для доступа только к test_v1()
class Tester_v1
{
public:
    static void run(Entity& e)
    {
        e.test_v1();
    }
};

// "Адвокат" для доступа только к test_v2()
class Tester_v2
{
public:
    static void run(Entity& e)
    {
        e.test_v2();
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Entity e;

    // Тестируем только через "адвокатов"
    Tester_v1::run(e);
    assert(e.value() == 42);

    Tester_v2::run(e);
    assert(e.value() == 84);

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
