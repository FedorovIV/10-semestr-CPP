#include <iostream>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>

class FibonacciIterator
{
public:
    FibonacciIterator()
        : m_current(0)
        , m_next(1)
        , m_index(0)
    {
    }

    explicit FibonacciIterator(int index)
        : FibonacciIterator()
    {
        while (m_index < index)
        {
            ++(*this);
        }
    }

    FibonacciIterator& operator++()
    {
        int const new_next = m_current + m_next;
        m_current = m_next;
        m_next = new_next;
        ++m_index;
        return *this;
    }

    FibonacciIterator operator++(int)
    {
        FibonacciIterator copy = *this;
        ++(*this);
        return copy;
    }

    int operator*() const
    {
        return m_current;
    }

    friend bool operator==(FibonacciIterator const& lhs, FibonacciIterator const& rhs)
    {
        return lhs.m_index == rhs.m_index;
    }

private:
    int m_current;
    int m_next;
    int m_index = 0;
};

class FibonacciFacadeIterator
    : public boost::iterator_facade<FibonacciFacadeIterator, int, boost::forward_traversal_tag, int>
{
public:
    FibonacciFacadeIterator()
        : m_current(0)
        , m_next(1)
        , m_index(0)
    {
    }

    explicit FibonacciFacadeIterator(int index)
        : FibonacciFacadeIterator()
    {
        while (m_index < index)
        {
            increment();
        }
    }

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        int const new_next = m_current + m_next;
        m_current = m_next;
        m_next = new_next;
        ++m_index;
    }

    bool equal(FibonacciFacadeIterator const& other) const
    {
        return m_index == other.m_index;
    }

    int dereference() const
    {
        return m_current;
    }

    int m_current;
    int m_next;
    int m_index = 0;
};

template <typename Iterator>
std::vector<int> first_values(int count)
{
    std::vector<int> values;
    Iterator begin;
    Iterator end(count);
    for (Iterator current = begin; !(current == end); ++current)
    {
        values.push_back(*current);
    }
    return values;
}

int main()
{
    std::vector<int> const expected = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    if (first_values<FibonacciIterator>(10) != expected)
    {
        std::cerr << "Manual Fibonacci iterator failed\n";
        return 1;
    }
    if (first_values<FibonacciFacadeIterator>(10) != expected)
    {
        std::cerr << "Boost facade Fibonacci iterator failed\n";
        return 2;
    }

    std::cout << "09.06: Fibonacci iterators work\n";
    return 0;
}
