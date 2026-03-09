////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Vector
{
public:
    // -------------------------------------------------------------------------
    Vector() : m_array(nullptr), m_size(0), m_capacity(0)
    {
        std::cout << "Vector::Vector (1)\n";
    }

    // -------------------------------------------------------------------------
    Vector(std::initializer_list<T> list)
        : m_size(list.size()), m_capacity(list.size())
    {
        std::cout << "Vector::Vector (2)\n";
        m_array = m_capacity ? new T[m_capacity]{} : nullptr;
        std::ranges::copy(list, m_array);
    }

    // -------------------------------------------------------------------------
    Vector(Vector const& other)
        : m_size(other.m_size), m_capacity(other.m_capacity)
    {
        std::cout << "Vector::Vector (3)\n";
        m_array = m_capacity ? new T[m_capacity]{} : nullptr;
        std::ranges::copy(other.m_array, other.m_array + m_size, m_array);
    }

    // -------------------------------------------------------------------------
    Vector(Vector&& other) noexcept
        : m_array(std::exchange(other.m_array, nullptr)),
          m_size(std::exchange(other.m_size, 0)),
          m_capacity(std::exchange(other.m_capacity, 0))
    {
        std::cout << "Vector::Vector (4)\n";
    }

    // -------------------------------------------------------------------------
    ~Vector()
    {
        std::cout << "Vector::~Vector\n";
        delete[] m_array;
    }

    // -------------------------------------------------------------------------
    Vector& operator=(Vector other)
    {
        std::cout << "Vector::operator=\n";
        swap(other);
        return *this;
    }

    // -------------------------------------------------------------------------
    void swap(Vector& other) noexcept
    {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    // -------------------------------------------------------------------------
    void push_back(T const& value)
    {
        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        m_array[m_size++] = value;
    }

    // -------------------------------------------------------------------------
    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= m_capacity)
            return;

        T* new_array = new T[new_capacity]{};
        std::ranges::copy(m_array, m_array + m_size, new_array);
        delete[] m_array;

        m_array = new_array;
        m_capacity = new_capacity;
    }

    // -------------------------------------------------------------------------
    void clear()
    {
        m_size = 0;
    }

    // -------------------------------------------------------------------------
    bool empty() const
    {
        return m_size == 0;
    }

    // -------------------------------------------------------------------------
    std::size_t size() const { return m_size; }
    std::size_t capacity() const { return m_capacity; }

    // -------------------------------------------------------------------------
    T const& operator[](std::size_t index) const
    {
        return m_array[index];
    }

private:
    T* m_array = nullptr;
    std::size_t m_size = 0;
    std::size_t m_capacity = 0;
};

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs)
{
    lhs.swap(rhs);
}

////////////////////////////////////////////////////////////////////////////////////
// ТЕСТЫ
////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // ---------------- int ----------------
    {
        Vector<int> v;

        for (int i = 1; i <= 10; ++i)
            v.push_back(i);

        assert(v.size() == 10);
        assert(v[0] == 1);
        assert(v[9] == 10);
    }

    // ---------------- double ----------------
    {
        Vector<double> v = {1.1, 2.2, 3.3};

        v.push_back(4.4);

        assert(v.size() == 4);
        assert(v[3] == 4.4);
    }

    // ---------------- std::string ----------------
    {
        Vector<std::string> v;
        v.push_back("hello");
        v.push_back("world");

        assert(v.size() == 2);
        assert(v[0] == "hello");
        assert(v[1] == "world");
    }

    std::cout << "All tests passed!\n";
}

////////////////////////////////////////////////////////////////////////////////////
