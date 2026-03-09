#include <cassert>
#include <cstddef>
#include <exception>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

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

class Exception : public std::exception
{
public:
    explicit Exception(std::string message)
        : m_message(std::move(message))
    {
    }

    char const* what() const noexcept override
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

class Rational final
    : public addable<Rational>
    , public subtractable<Rational>
    , public multipliable<Rational>
    , public dividable<Rational>
{
public:
    Rational(int numerator = 0, int denominator = 1)
        : m_numerator(numerator)
        , m_denominator(denominator)
    {
        if (m_denominator == 0)
        {
            throw Exception("Rational: zero denominator");
        }
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
        if (other.m_numerator == 0)
        {
            throw Exception("Rational: division by zero");
        }
        m_numerator *= other.m_denominator;
        m_denominator *= other.m_numerator;
        normalize();
        return *this;
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

void demonstrate_standard_exceptions()
{
    try
    {
        // Deterministic simulation of an allocation failure for demonstration.
        throw std::bad_alloc();
    }
    catch (std::bad_alloc const& error)
    {
        std::cerr << "std::bad_alloc: " << error.what()
                  << " (memory allocation failed)\n";
    }

    try
    {
        std::variant<int, std::string> value = 42;
        (void)std::get<std::string>(value);
    }
    catch (std::bad_variant_access const& error)
    {
        std::cerr << "std::bad_variant_access: " << error.what()
                  << " (requested wrong alternative from std::variant)\n";
    }

    try
    {
        std::optional<int> value;
        (void)value.value();
    }
    catch (std::bad_optional_access const& error)
    {
        std::cerr << "std::bad_optional_access: " << error.what()
                  << " (attempted value() on empty std::optional)\n";
    }

    try
    {
        std::vector<int> values;
        values.reserve(values.max_size() + 1);
    }
    catch (std::length_error const& error)
    {
        std::cerr << "std::length_error: " << error.what()
                  << " (requested vector capacity above max_size)\n";
    }

    try
    {
        std::vector<int> values = {1, 2, 3};
        (void)values.at(10);
    }
    catch (std::out_of_range const& error)
    {
        std::cerr << "std::out_of_range: " << error.what()
                  << " (vector::at index is outside current size)\n";
    }
}

int main()
{
	// Sample comment to check git push
    try
    {
        Rational a(1, 2);
        Rational b(1, 3);
        Rational c = (a + b) * Rational(6, 1);
        (void)c;

        bool caught_custom = false;
        try
        {
            Rational invalid(1, 0);
            (void)invalid;
        }
        catch (Exception const& error)
        {
            caught_custom = true;
            std::cerr << "Custom exception: " << error.what() << '\n';
        }
        if (!caught_custom)
        {
            std::cerr << "Expected custom exception was not raised\n";
            return 4;
        }

        demonstrate_standard_exceptions();
        std::cout << "07.02: demonstration completed\n";
    }
    catch (std::exception const& error)
    {
        std::cerr << "main caught std::exception: " << error.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cerr << "main caught unknown exception\n";
        return 2;
    }

    return 0;
}
