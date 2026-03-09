///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cmath>
#include <iostream>
#include <numbers>

///////////////////////////////////////////////////////////////////////////////////////////////

class Triangle
{
public:
    Triangle(double a, double b, double c) : m_a(a), m_b(b), m_c(c) {}

    double perimeter() const
    {
        return m_a + m_b + m_c;
    }

    double area() const
    {
        double p = perimeter() / 2.0;
        return std::sqrt(p * (p - m_a) * (p - m_b) * (p - m_c)); // формула Герона
    }

private:
    double m_a = 0.0, m_b = 0.0, m_c = 0.0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Square
{
public:
    explicit Square(double side) : m_side(side) {}

    double perimeter() const
    {
        return 4.0 * m_side;
    }

    double area() const
    {
        return m_side * m_side;
    }

private:
    double m_side = 0.0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Circle
{
public:
    explicit Circle(double radius) : m_radius(radius) {}

    double perimeter() const
    {
        return 2.0 * std::numbers::pi * m_radius;
    }

    double area() const
    {
        return std::numbers::pi * m_radius * m_radius;
    }

private:
    double m_radius = 0.0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Проверка треугольника
    {
        Triangle t(3, 4, 5);
        assert(std::abs(t.perimeter() - 12.0) < 1e-6);
        assert(std::abs(t.area() - 6.0) < 1e-6);
    }

    // Проверка квадрата
    {
        Square s(5);
        assert(std::abs(s.perimeter() - 20.0) < 1e-6);
        assert(std::abs(s.area() - 25.0) < 1e-6);
    }

    // Проверка круга
    {
        Circle c(2);
        assert(std::abs(c.perimeter() - (2 * std::numbers::pi * 2)) < 1e-6);
        assert(std::abs(c.area() - (std::numbers::pi * 2 * 2)) < 1e-6);
    }

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
