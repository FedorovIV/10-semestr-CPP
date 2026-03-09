//////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////
// Template Method — логика хода
//////////////////////////////////////////////////////////////////

class TurnLogic
{
public:
    virtual ~TurnLogic() = default;

    void take_turn()
    {
        begin_turn();
        perform_action();
        end_turn();
    }

protected:
    virtual void begin_turn()   {}
    virtual void perform_action() = 0;
    virtual void end_turn()     {}
};

//////////////////////////////////////////////////////////////////
// GameObject — базовый элемент иерархии (Composite)
//////////////////////////////////////////////////////////////////

class GameObject : public TurnLogic
{
public:
    virtual ~GameObject() = default;
    virtual int power() const = 0;
};

//////////////////////////////////////////////////////////////////
// Leaf — Unit
//////////////////////////////////////////////////////////////////

class Unit : public GameObject
{
public:
    Unit(std::string name, int attack)
        : m_name(std::move(name)), m_attack(attack)
    {}

    int power() const override
    {
        return m_attack;
    }

protected:
    void perform_action() override
    {
        std::cout << m_name << " attacks with power " << m_attack << '\n';
    }

private:
    std::string m_name;
    int m_attack;
};

//////////////////////////////////////////////////////////////////
// Composite — Army
//////////////////////////////////////////////////////////////////

class Army : public GameObject
{
public:
    void add(std::shared_ptr<GameObject> obj)
    {
        m_units.push_back(std::move(obj));
    }

    int power() const override
    {
        int total = 0;
        for (auto const& u : m_units)
            total += u->power();
        return total;
    }

protected:
    void perform_action() override
    {
        for (auto const& u : m_units)
            u->take_turn();
    }

private:
    std::vector<std::shared_ptr<GameObject>> m_units;
};

//////////////////////////////////////////////////////////////////
// Builder — создание Unit
//////////////////////////////////////////////////////////////////

class UnitBuilder
{
public:
    UnitBuilder& name(std::string n)
    {
        m_name = std::move(n);
        return *this;
    }

    UnitBuilder& attack(int a)
    {
        m_attack = a;
        return *this;
    }

    std::shared_ptr<Unit> build() const
    {
        return std::make_shared<Unit>(m_name, m_attack);
    }

private:
    std::string m_name = "Unknown";
    int m_attack = 0;
};

//////////////////////////////////////////////////////////////////
// Tests / Demo
//////////////////////////////////////////////////////////////////

int main()
{
    UnitBuilder builder;

    auto soldier = builder.name("Soldier").attack(10).build();
    auto knight  = builder.name("Knight").attack(20).build();

    Army army;
    army.add(soldier);
    army.add(knight);

    assert(soldier->power() == 10);
    assert(knight->power()  == 20);
    assert(army.power()     == 30);

    army.take_turn(); // Template Method

    std::cout << "All tests passed\n";
}

//////////////////////////////////////////////////////////////////
