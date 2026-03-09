///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <numeric>  // std::gcd, std::lcm

///////////////////////////////////////////////////////////////////////////////////////////////

// Рекурсивный алгоритм НОД (по алгоритму Евклида)
int gcd_recursive(int a, int b)
{
    return b == 0 ? a : gcd_recursive(b, a % b);
}

// Итеративный алгоритм НОД (тот же Евклид без рекурсии)
int gcd_iterative(int a, int b)
{
    while (b != 0)
    {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

// НОК через НОД
int lcm_custom(int a, int b)
{
    return a / gcd_iterative(a, b) * b;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Тестовые данные
    int a = 42, b = 30;

    // Проверка рекурсивного НОД
    assert(gcd_recursive(a, b) == std::gcd(a, b));

    // Проверка итеративного НОД
    assert(gcd_iterative(a, b) == std::gcd(a, b));

    // Проверка НОК
    assert(lcm_custom(a, b) == std::lcm(a, b));

    std::cout << "a = " << a << ", b = " << b << '\n';
    std::cout << "gcd_recursive = " << gcd_recursive(a, b) << '\n';
    std::cout << "gcd_iterative = " << gcd_iterative(a, b) << '\n';
    std::cout << "lcm_custom    = " << lcm_custom(a, b) << '\n';

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
