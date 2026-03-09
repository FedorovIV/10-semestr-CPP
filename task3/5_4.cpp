//////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>

//////////////////////////////////////////////////////////////////
// Concrete strategies (NO base Strategy)
//////////////////////////////////////////////////////////////////

struct Aggressive
{
    void act() const
    {
        std::cout << "Aggressive attack\n";
    }
};

struct Defensive
{
    void act() const
    {
        std::cout << "Defensive stance\n";
    }
};

//////////////////////////////////////////////////////////////////
// Entity — static Strategy (CRTP-style)
//////////////////////////////////////////////////////////////////

template <typename Strategy>
class Entity : private Strategy
{
public:
    void update() const
    {
        // static polymorphism
        static_cast<Strategy const&>(*this).act();
    }
};

//////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////
int main()
{
    Entity<Aggressive> warrior;
    Entity<Defensive>  guard;

    warrior.update(); // Aggressive attack
    guard.update();   // Defensive stance

    static_assert(std::is_empty_v<Entity<Aggressive>>);
    static_assert(std::is_empty_v<Entity<Defensive>>);

    std::cout << "All tests passed\n";
}


//////////////////////////////////////////////////////////////////
