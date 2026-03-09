///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

// Абстрактный базовый класс
class Shape
{
public:
    virtual ~Shape() = default;

    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Triangle final : public Shape
{
public:
    Triangle(double a, double b, double c)
        : m_a(a), m_b(b), m_c(c) {}

    double perimeter() const override final
    {
        return m_a + m_b + m_c;
    }

    double area() const override final
    {
        double p = perimeter() / 2.0;
        return std::sqrt(p * (p - m_a) * (p - m_b) * (p - m_c)); // формула Герона
    }

private:
    double m_a = 0.0, m_b = 0.0, m_c = 0.0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Square final : public Shape
{
public:
    explicit Square(double side)
        : m_side(side) {}

    double perimeter() const override final
    {
        return 4.0 * m_side;
    }

    double area() const override final
    {
        return m_side * m_side;
    }

private:
    double m_side = 0.0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Circle final : public Shape
{
public:
    explicit Circle(double radius)
        : m_radius(radius) {}

    double perimeter() const override final
    {
        return 2.0 * std::numbers::pi * m_radius;
    }

    double area() const override final
    {
        return std::numbers::pi * m_radius * m_radius;
    }

private:
    double m_radius = 0.0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.emplace_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.emplace_back(std::make_unique<Square>(5));
    shapes.emplace_back(std::make_unique<Circle>(2));

    // Проверим корректность расчётов
    assert(std::abs(shapes[0]->perimeter() - 12.0) < 1e-6);
    assert(std::abs(shapes[0]->area() - 6.0) < 1e-6);

    assert(std::abs(shapes[1]->perimeter() - 20.0) < 1e-6);
    assert(std::abs(shapes[1]->area() - 25.0) < 1e-6);

    assert(std::abs(shapes[2]->perimeter() - (2 * std::numbers::pi * 2)) < 1e-6);
    assert(std::abs(shapes[2]->area() - (std::numbers::pi * 2 * 2)) < 1e-6);

    std::cout << "All tests passed!\n";

    // Демонстрация полиморфизма
    std::cout << "\nShapes info:\n";
    for (auto const& s : shapes)
    {
        std::cout << "Perimeter: " << s->perimeter()
                  << ", Area: " << s->area() << '\n';
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
