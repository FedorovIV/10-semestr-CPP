#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>

class Entity_v1
{
public:
    explicit Entity_v1(int value)
        : m_value(value)
    {
    }

    int value() const
    {
        return m_value;
    }

private:
    int m_value = 0;
};

class Entity_v2
{
public:
    int value = 0;
};

void hack_with_reinterpret_cast(Entity_v1& entity, int new_value)
{
    auto& alias = reinterpret_cast<Entity_v2&>(entity);
    alias.value = new_value;
}

void hack_with_binary_patch(Entity_v1& entity, int new_value)
{
    // We patch the first bytes of the object directly, assuming int field is at offset 0.
    std::memcpy(static_cast<void*>(&entity), &new_value, sizeof(new_value));
}

int main()
{
    Entity_v1 object(10);
    assert(object.value() == 10);

    hack_with_reinterpret_cast(object, 42);
    assert(object.value() == 42);

    hack_with_binary_patch(object, -7);
    assert(object.value() == -7);

    std::cout << "08.01: private field was modified by external code\n";
    return 0;
}
