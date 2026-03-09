#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

using Roots = std::variant<double, std::pair<double, double>, std::monostate>;

std::optional<Roots> solve(double a, double b, double c)
{
    constexpr double epsilon = 1e-9;

    if (std::abs(a) < epsilon)
    {
        if (std::abs(b) < epsilon)
        {
            if (std::abs(c) < epsilon)
            {
                return Roots(std::monostate{});
            }
            return std::nullopt;
        }
        return Roots(-c / b);
    }

    double const discriminant = b * b - 4.0 * a * c;
    if (discriminant < -epsilon)
    {
        return std::nullopt;
    }
    if (std::abs(discriminant) < epsilon)
    {
        return Roots(-b / (2.0 * a));
    }

    double const root = std::sqrt(discriminant);
    return Roots(std::pair<double, double>{
        (-b - root) / (2.0 * a),
        (-b + root) / (2.0 * a)
    });
}

void print_roots(std::optional<Roots> const& roots)
{
    if (!roots.has_value())
    {
        std::cout << "No roots\n";
        return;
    }

    if (std::holds_alternative<double>(*roots))
    {
        std::cout << "One root: " << std::get<double>(*roots) << '\n';
        return;
    }

    if (std::holds_alternative<std::pair<double, double>>(*roots))
    {
        auto const pair = std::get<std::pair<double, double>>(*roots);
        std::cout << "Two roots: " << pair.first << ' ' << pair.second << '\n';
        return;
    }

    std::cout << "Infinitely many roots\n";
}

int main()
{
    {
        auto roots = solve(1.0, -3.0, 2.0);
        if (!roots.has_value() || !std::holds_alternative<std::pair<double, double>>(*roots))
        {
            std::cerr << "Unexpected result for equation x^2 - 3x + 2 = 0\n";
            return 1;
        }
        auto values = std::get<std::pair<double, double>>(*roots);
        if (std::abs(values.first - 1.0) >= 1e-9 || std::abs(values.second - 2.0) >= 1e-9)
        {
            std::cerr << "Incorrect roots for equation x^2 - 3x + 2 = 0\n";
            return 2;
        }
    }

    {
        auto roots = solve(1.0, 2.0, 1.0);
        if (!roots.has_value() || !std::holds_alternative<double>(*roots))
        {
            std::cerr << "Unexpected result for equation x^2 + 2x + 1 = 0\n";
            return 3;
        }
        if (std::abs(std::get<double>(*roots) + 1.0) >= 1e-9)
        {
            std::cerr << "Incorrect root for equation x^2 + 2x + 1 = 0\n";
            return 4;
        }
    }

    {
        auto roots = solve(0.0, 0.0, 0.0);
        if (!roots.has_value() || !std::holds_alternative<std::monostate>(*roots))
        {
            std::cerr << "Expected infinitely many roots for 0 = 0\n";
            return 5;
        }
    }

    {
        auto roots = solve(0.0, 0.0, 1.0);
        if (roots.has_value())
        {
            std::cerr << "Expected no roots for equation 1 = 0\n";
            return 6;
        }
    }

    {
        auto roots = solve(1.0, 0.0, 1.0);
        if (roots.has_value())
        {
            std::cerr << "Expected no real roots for x^2 + 1 = 0\n";
            return 7;
        }
    }

    std::cout << "07.01: all tests passed\n";

    std::cout << "Enter a, b, c: ";
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    if (std::cin >> a >> b >> c)
    {
        print_roots(solve(a, b, c));
    }

    return 0;
}
