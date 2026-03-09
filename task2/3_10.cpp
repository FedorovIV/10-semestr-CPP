///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational
{
public:
    /* explicit */ Rational(int num = 0, int den = 1) : m_num(num), m_den(den)
    {
        reduce();
    }

    // ----------------------------------------------------------------------------------------

    explicit operator double() const
    {
        return 1.0 * m_num / m_den;
    }

    // ----------------------------------------------------------------------------------------

    auto &operator+=(Rational const &other)
    {
        auto lcm = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;
        reduce();
        return *this;
    }

    // ----------------------------------------------------------------------------------------

    auto &operator-=(Rational const &other)
    {
        return *this += Rational(-other.m_num, other.m_den);
    }

    // ----------------------------------------------------------------------------------------

    auto &operator*=(Rational const &other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    // ----------------------------------------------------------------------------------------

    auto &operator/=(Rational const &other)
    {
        return *this *= Rational(other.m_den, other.m_num);
    }

    // ----------------------------------------------------------------------------------------

    auto const operator++(int)
    {
        auto x = *this;
        *this += 1;
        return x;
    }
    auto const operator--(int)
    {
        auto x = *this;
        *this -= 1;
        return x;
    }

    auto &operator++()
    {
        *this += 1;
        return *this;
    }
    auto &operator--()
    {
        *this -= 1;
        return *this;
    }

    // ----------------------------------------------------------------------------------------

    friend auto operator+(Rational lhs, Rational const &rhs) { return lhs += rhs; }
    friend auto operator-(Rational lhs, Rational const &rhs) { return lhs -= rhs; }
    friend auto operator*(Rational lhs, Rational const &rhs) { return lhs *= rhs; }
    friend auto operator/(Rational lhs, Rational const &rhs) { return lhs /= rhs; }

    // ----------------------------------------------------------------------------------------
    // 💡 Главное отличие задачи 03.10 — используем <=> вместо 6 операторов
    // ----------------------------------------------------------------------------------------

    friend std::strong_ordering operator<=>(Rational const &lhs, Rational const &rhs)
    {
        auto left = static_cast<long long>(lhs.m_num) * rhs.m_den;
        auto right = static_cast<long long>(rhs.m_num) * lhs.m_den;

        if (left < right)
            return std::strong_ordering::less;
        if (left > right)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    // Равенство теперь определяется через <=> (одно условие)
    friend bool operator==(Rational const &lhs, Rational const &rhs)
    {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    // ----------------------------------------------------------------------------------------

    friend std::istream &operator>>(std::istream &stream, Rational &rational)
    {
        char slash;
        stream >> rational.m_num >> slash >> rational.m_den;
        rational.reduce();
        return stream;
    }

    friend std::ostream &operator<<(std::ostream &stream, Rational const &rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:
    void reduce()
    {
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }
        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }

    int m_num = 0, m_den = 1;
};

///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Rational x = 1, y(2, 1);

    assert(equal(static_cast<double>(x), 1.0));

    assert((x += y) == Rational(3, 1));
    assert((x -= y) == Rational(1, 1));
    assert((x *= y) == Rational(2, 1));
    assert((x /= y) == Rational(1, 1));

    assert((x++) == Rational(1, 1));
    assert((x--) == Rational(2, 1));
    assert((++y) == Rational(3, 1));
    assert((--y) == Rational(2, 1));

    assert((x + y) == Rational(3, 1));
    assert((x - y) == Rational(-1, 1));
    assert((x * y) == Rational(2, 1));
    assert((x / y) == Rational(1, 2));

    assert((x <=> y) == std::strong_ordering::less);

    Rational x2(1, 2);
    Rational y2(2, 4);
    assert((x2 == y2));

    std::stringstream stream_1("1/2");
    std::stringstream stream_2;

    stream_1 >> x;
    stream_2 << x;

    assert(stream_2.str() == stream_1.str());

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
