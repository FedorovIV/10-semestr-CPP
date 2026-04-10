#include <cstddef>
#include <iostream>
#include <new>

template <typename Derived>
class Entity
{
public:
    static void* operator new(std::size_t size)
    {
        ++s_new_calls;
        return ::operator new(size);
    }

    static void operator delete(void* pointer) noexcept
    {
        ++s_delete_calls;
        ::operator delete(pointer);
    }

    static void* operator new[](std::size_t size)
    {
        ++s_new_array_calls;
        return ::operator new[](size);
    }

    static void operator delete[](void* pointer) noexcept
    {
        ++s_delete_array_calls;
        ::operator delete[](pointer);
    }

    static void* operator new(std::size_t size, std::nothrow_t const& tag) noexcept
    {
        ++s_nothrow_new_calls;
        return ::operator new(size, tag);
    }

    static void operator delete(void* pointer, std::nothrow_t const&) noexcept
    {
        ++s_nothrow_delete_calls;
        ::operator delete(pointer);
    }

    static void* operator new[](std::size_t size, std::nothrow_t const& tag) noexcept
    {
        ++s_nothrow_new_array_calls;
        return ::operator new[](size, tag);
    }

    static void operator delete[](void* pointer, std::nothrow_t const&) noexcept
    {
        ++s_nothrow_delete_array_calls;
        ::operator delete[](pointer);
    }

    static int new_calls()
    {
        return s_new_calls;
    }

    static int delete_calls()
    {
        return s_delete_calls;
    }

    static int new_array_calls()
    {
        return s_new_array_calls;
    }

    static int delete_array_calls()
    {
        return s_delete_array_calls;
    }

    static int nothrow_new_calls()
    {
        return s_nothrow_new_calls;
    }

    static int nothrow_new_array_calls()
    {
        return s_nothrow_new_array_calls;
    }

protected:
    Entity() = default;

private:
    static inline int s_new_calls = 0;
    static inline int s_delete_calls = 0;
    static inline int s_new_array_calls = 0;
    static inline int s_delete_array_calls = 0;
    static inline int s_nothrow_new_calls = 0;
    static inline int s_nothrow_delete_calls = 0;
    static inline int s_nothrow_new_array_calls = 0;
    static inline int s_nothrow_delete_array_calls = 0;
};

class Client : private Entity<Client>
{
public:
    using Entity<Client>::delete_array_calls;
    using Entity<Client>::delete_calls;
    using Entity<Client>::new_array_calls;
    using Entity<Client>::new_calls;
    using Entity<Client>::nothrow_new_array_calls;
    using Entity<Client>::nothrow_new_calls;
    using Entity<Client>::operator delete;
    using Entity<Client>::operator delete[];
    using Entity<Client>::operator new;
    using Entity<Client>::operator new[];
};

int main()
{
    Client* single = new Client;
    delete single;

    Client* array = new Client[3];
    delete[] array;

    Client* single_nothrow = new (std::nothrow) Client;
    delete single_nothrow;

    Client* array_nothrow = new (std::nothrow) Client[2];
    delete[] array_nothrow;

    if (Client::new_calls() < 1 || Client::delete_calls() < 1)
    {
        std::cerr << "Scalar operator new/delete were not used\n";
        return 1;
    }
    if (Client::new_array_calls() < 1 || Client::delete_array_calls() < 1)
    {
        std::cerr << "Array operator new[]/delete[] were not used\n";
        return 2;
    }
    if (Client::nothrow_new_calls() < 1 || Client::nothrow_new_array_calls() < 1)
    {
        std::cerr << "Nothrow overloads were not used\n";
        return 3;
    }

    std::cout << "09.08: custom allocation operators work\n";
    return 0;
}
