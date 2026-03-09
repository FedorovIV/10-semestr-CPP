#include <iostream>
#include <cassert>
#include "rational.hpp"

int main() {
    Rational r1(1, 2);
    Rational r2(1, 3);

    Rational sum = r1.add(r2);
    assert(sum.getNum() == 5 && sum.getDen() == 6);

    Rational diff = r1.sub(r2);
    assert(diff.getNum() == 1 && diff.getDen() == 6);

    Rational prod = r1.mul(r2);
    assert(prod.getNum() == 1 && prod.getDen() == 6);

    Rational quot = r1.div(r2);
    assert(quot.getNum() == 3 && quot.getDen() == 2);

    std::cout << "06.01: All tests passed." << std::endl;

    return 0;
}