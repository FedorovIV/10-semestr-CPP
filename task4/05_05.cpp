#include <cassert>
#include <compare>
#include <iostream>
#include <numeric>
#include <sstream>

template <typename Derived>
struct addable
{
    friend Derived operator+(Derived lhs, Derived const& rhs)
    {
        lhs += rhs;
        return lhs;
    }
};

template <typename Derived>
struct subtractable
{
    friend Derived operator-(Derived lhs, Derived const& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
};

template <typename Derived>
struct multipliable
{
    friend Derived operator*(Derived lhs, Derived const& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
};

template <typename Derived>
struct dividable
{
    friend Derived operator/(Derived lhs, Derived const& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
};

template <typename Derived>
struct incrementable
{
    friend Derived& operator++(Derived& value)
    {
        value += Derived(1);
        return value;
    }

    friend Derived operator++(Derived& value, int)
    {
        Derived old = value;
        ++value;
        return old;
    }
};

template <typename Derived>
struct decrementable
{
    friend Derived& operator--(Derived& value)
    {
        value -= Derived(1);
        return value;
    }

    friend Derived operator--(Derived& value, int)
    {
        Derived old = value;
        --value;
        return old;
    }
};

class Rational final
    : public addable<Rational>
    , public subtractable<Rational>
    , public multipliable<Rational>
    , public dividable<Rational>
    , public incrementable<Rational>
    , public decrementable<Rational>
{
public:
    Rational(int numerator = 0, int denominator = 1)
        : m_numerator(numerator)
        , m_denominator(denominator)
    {
        assert(m_denominator != 0);
        normalize();
    }

    Rational& operator+=(Rational const& other)
    {
        int const common = std::lcm(m_denominator, other.m_denominator);
        m_numerator = m_numerator * (common / m_denominator)
                    + other.m_numerator * (common / other.m_denominator);
        m_denominator = common;
        normalize();
        return *this;
    }

    Rational& operator-=(Rational const& other)
    {
        return *this += Rational(-other.m_numerator, other.m_denominator);
    }

    Rational& operator*=(Rational const& other)
    {
        m_numerator *= other.m_numerator;
        m_denominator *= other.m_denominator;
        normalize();
        return *this;
    }

    Rational& operator/=(Rational const& other)
    {
        assert(other.m_numerator != 0);
        m_numerator *= other.m_denominator;
        m_denominator *= other.m_numerator;
        normalize();
        return *this;
    }

    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs)
    {
        long long const left = static_cast<long long>(lhs.m_numerator) * rhs.m_denominator;
        long long const right = static_cast<long long>(rhs.m_numerator) * lhs.m_denominator;
        if (left < right)
        {
            return std::strong_ordering::less;
        }
        if (left > right)
        {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const& lhs, Rational const& rhs)
    {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend std::ostream& operator<<(std::ostream& out, Rational const& value)
    {
        out << value.m_numerator << '/' << value.m_denominator;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Rational& value)
    {
        int numerator = 0;
        int denominator = 1;
        char slash = '\0';
        in >> numerator >> slash >> denominator;
        if (in && slash == '/')
        {
            value = Rational(numerator, denominator);
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

private:
    void normalize()
    {
        if (m_denominator < 0)
        {
            m_numerator = -m_numerator;
            m_denominator = -m_denominator;
        }
        int const divisor = std::gcd(m_numerator, m_denominator);
        m_numerator /= divisor;
        m_denominator /= divisor;
    }

    int m_numerator = 0;
    int m_denominator = 1;
};

int main()
{
    Rational x(1, 2);
    Rational y(2, 3);

    assert((x + y) == Rational(7, 6));
    assert((x - y) == Rational(-1, 6));
    assert((x * y) == Rational(1, 3));
    assert((x / y) == Rational(3, 4));

    Rational z(5, 4);
    assert((z++) == Rational(5, 4));
    assert(z == Rational(9, 4));
    assert((--z) == Rational(5, 4));

    std::stringstream input("8/12");
    Rational parsed;
    input >> parsed;
    assert(parsed == Rational(2, 3));

    std::cout << "05.05: all tests passed\n";
    return 0;
}
