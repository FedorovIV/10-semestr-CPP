#include <iostream>
#include <array>
#include <cmath>

consteval double calculate_pi() {
    double pi = 0.0;
    for (int i = 0; i < 10000; ++i) {
        pi += 4.0 / (2 * i + 1) * (i % 2 == 0 ? 1 : -1);
    }
    return pi;
}

consteval double calculate_e() {
    double e = 1.0;
    double factorial = 1.0;
    for (int i = 1; i < 15; ++i) {
        factorial *= i;
        e += 1.0 / factorial;
    }
    return e;
}

constexpr double pi = calculate_pi();
constexpr double e = calculate_e();

constexpr std::array<double, 5> epsilon_values = {
    0.1,
    0.01,
    0.001,
    0.0001,
    0.00001
};

int main() {
    std::cout << "Pi (calculated at compile time) = " << pi << std::endl;
    std::cout << "E (calculated at compile time) = " << e << std::endl;
    std::cout << "\nEpsilon values:" << std::endl;
    
    for (size_t i = 0; i < epsilon_values.size(); ++i) {
        std::cout << "epsilon[" << i << "] = " << epsilon_values[i] << std::endl;
    }
    
    std::cout << "\nComparison with standard values:" << std::endl;
    std::cout << "Difference from M_PI: " << std::abs(pi - M_PI) << std::endl;
    std::cout << "Difference from M_E: " << std::abs(e - M_E) << std::endl;
    
    return 0;
}