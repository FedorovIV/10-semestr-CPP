#include <array>
#include <bit>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>

class Entity
{
private:
    struct Implementation;

public:
    Entity(int value, double factor)
    {
        static_assert(sizeof(Implementation) <= storage_size);
        static_assert(alignof(Implementation) <= alignof(std::max_align_t));
        new (m_storage.data()) Implementation(value, factor);
    }

    ~Entity()
    {
        std::destroy_at(get());
    }

    void set_value(int value)
    {
        get()->value = value;
    }

    int value() const
    {
        return get()->value;
    }

    double scaled() const
    {
        return get()->value * get()->factor;
    }

    Implementation* get()
    {
        return std::launder(std::bit_cast<Implementation*>(m_storage.data()));
    }

    Implementation const* get() const
    {
        return std::launder(std::bit_cast<Implementation const*>(m_storage.data()));
    }

private:
    struct Implementation
    {
        Implementation(int value, double factor)
            : value(value)
            , factor(factor)
        {
        }

        int value = 0;
        double factor = 1.0;
    };

    static constexpr std::size_t storage_size = 16;

    alignas(std::max_align_t) std::array<std::byte, storage_size> m_storage{};
};

int main()
{
    Entity entity(7, 1.5);
    if (entity.value() != 7)
    {
        std::cerr << "Initial value is incorrect\n";
        return 1;
    }
    entity.set_value(10);
    if (entity.value() != 10)
    {
        std::cerr << "Pimpl update failed\n";
        return 2;
    }
    if (entity.scaled() != 15.0)
    {
        std::cerr << "Scaled value is incorrect\n";
        return 3;
    }

    std::cout << "09.07: inline Pimpl works\n";
    return 0;
}
