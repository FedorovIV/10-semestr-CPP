#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>

using Matrix = boost::numeric::ublas::matrix<unsigned long long>;

Matrix identity_matrix()
{
    Matrix result(2, 2);
    result(0, 0) = 1;
    result(0, 1) = 0;
    result(1, 0) = 0;
    result(1, 1) = 1;
    return result;
}

Matrix multiply(Matrix const& lhs, Matrix const& rhs)
{
    Matrix result(2, 2);
    for (std::size_t row = 0; row < 2; ++row)
    {
        for (std::size_t column = 0; column < 2; ++column)
        {
            result(row, column) = 0;
            for (std::size_t k = 0; k < 2; ++k)
            {
                result(row, column) += lhs(row, k) * rhs(k, column);
            }
        }
    }
    return result;
}

Matrix power(Matrix base, unsigned long long exponent)
{
    Matrix result = identity_matrix();
    while (exponent > 0)
    {
        if ((exponent & 1ULL) != 0ULL)
        {
            result = multiply(result, base);
        }
        exponent >>= 1ULL;
        if (exponent > 0)
        {
            base = multiply(base, base);
        }
    }
    return result;
}

unsigned long long fibonacci(unsigned long long n)
{
    if (n == 0)
    {
        return 0;
    }

    Matrix base(2, 2);
    base(0, 0) = 1;
    base(0, 1) = 1;
    base(1, 0) = 1;
    base(1, 1) = 0;

    Matrix const result = power(base, n - 1);
    return result(0, 0);
}

int main()
{
    if (fibonacci(0) != 0 || fibonacci(1) != 1 || fibonacci(10) != 55)
    {
        std::cerr << "Matrix Fibonacci checks failed\n";
        return 1;
    }
    if (fibonacci(50) != 12586269025ULL)
    {
        std::cerr << "Unexpected Fibonacci(50)\n";
        return 2;
    }

    std::cout << "10.06: Fibonacci(20) = " << fibonacci(20) << '\n';
    std::cout << "10.06: asymptotic complexity is O(log N)\n";
    return 0;
}
