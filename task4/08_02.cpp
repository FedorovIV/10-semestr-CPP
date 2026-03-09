#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/multiprecision/cpp_int.hpp>

class Integer
{
public:
    using value_type = boost::multiprecision::cpp_int;

    Integer() = default;

    Integer(long long value)
        : m_value(value)
    {
    }

    Integer(std::string const& text)
    {
        std::stringstream stream(text);
        stream >> m_value;
        if (!stream || !stream.eof())
        {
            throw std::invalid_argument("Integer: invalid string representation");
        }
    }

    Integer& operator+=(Integer const& other)
    {
        m_value += other.m_value;
        return *this;
    }

    Integer& operator-=(Integer const& other)
    {
        m_value -= other.m_value;
        return *this;
    }

    Integer& operator*=(Integer const& other)
    {
        m_value *= other.m_value;
        return *this;
    }

    Integer& operator/=(Integer const& other)
    {
        if (other.m_value == 0)
        {
            throw std::invalid_argument("Integer: division by zero");
        }
        m_value /= other.m_value;
        return *this;
    }

    Integer& operator%=(Integer const& other)
    {
        if (other.m_value == 0)
        {
            throw std::invalid_argument("Integer: modulo by zero");
        }
        m_value %= other.m_value;
        return *this;
    }

    Integer operator++(int)
    {
        Integer old = *this;
        ++(*this);
        return old;
    }

    Integer operator--(int)
    {
        Integer old = *this;
        --(*this);
        return old;
    }

    Integer& operator++()
    {
        ++m_value;
        return *this;
    }

    Integer& operator--()
    {
        --m_value;
        return *this;
    }

    int sign() const
    {
        if (m_value == 0)
        {
            return 0;
        }
        return (m_value < 0) ? -1 : 1;
    }

    Integer abs() const
    {
        return (m_value < 0) ? Integer(-m_value) : *this;
    }

    friend Integer operator+(Integer lhs, Integer const& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Integer operator-(Integer lhs, Integer const& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend Integer operator*(Integer lhs, Integer const& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    friend Integer operator/(Integer lhs, Integer const& rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    friend Integer operator%(Integer lhs, Integer const& rhs)
    {
        lhs %= rhs;
        return lhs;
    }

    friend bool operator<(Integer const& lhs, Integer const& rhs)
    {
        return lhs.m_value < rhs.m_value;
    }

    friend bool operator>(Integer const& lhs, Integer const& rhs)
    {
        return rhs < lhs;
    }

    friend bool operator<=(Integer const& lhs, Integer const& rhs)
    {
        return !(rhs < lhs);
    }

    friend bool operator>=(Integer const& lhs, Integer const& rhs)
    {
        return !(lhs < rhs);
    }

    friend bool operator==(Integer const& lhs, Integer const& rhs)
    {
        return lhs.m_value == rhs.m_value;
    }

    friend bool operator!=(Integer const& lhs, Integer const& rhs)
    {
        return !(lhs == rhs);
    }

    friend std::istream& operator>>(std::istream& stream, Integer& value)
    {
        stream >> value.m_value;
        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, Integer const& value)
    {
        stream << value.m_value;
        return stream;
    }

    friend Integer pow(Integer base, unsigned int exponent)
    {
        Integer result(1);
        while (exponent > 0)
        {
            if ((exponent & 1U) != 0U)
            {
                result *= base;
            }
            exponent >>= 1U;
            if (exponent > 0)
            {
                base *= base;
            }
        }
        return result;
    }

private:
    explicit Integer(value_type value)
        : m_value(std::move(value))
    {
    }

    value_type m_value = 0;
};

int main()
{
    Integer x("11111111111111111111111111111111");
    Integer y("22222222222222222222222222222222");

    assert((x + y) == Integer("33333333333333333333333333333333"));
    assert((y - x) == Integer("11111111111111111111111111111111"));
    assert((x * Integer(3)) == Integer("33333333333333333333333333333333"));
    assert((y / Integer(2)) == Integer("11111111111111111111111111111111"));

    assert((y % x) == Integer("11111111111111111111111111111111"));
    assert((pow(Integer(2), 10)) == Integer(1024));

    Integer z(-123);
    assert(z.sign() == -1);
    assert(z.abs() == Integer(123));
    assert(Integer(0).sign() == 0);

    std::stringstream input("-77");
    Integer parsed;
    input >> parsed;
    assert(parsed == Integer(-77));

    std::cout << "08.02: all tests passed\n";
    return 0;
}
