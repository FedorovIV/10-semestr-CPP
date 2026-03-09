import <cassert>;
import <iostream>;

import rational;

using math::Rational;

int main() {
    // Базовые проверки арифметики.
    Rational r1(1, 2);
    Rational r2(1, 3);

    Rational sum = r1.add(r2);
    assert(sum.num() == 5 && sum.den() == 6);

    Rational diff = r1.sub(r2);
    assert(diff.num() == 1 && diff.den() == 6);

    Rational prod = r1.mul(r2);
    assert(prod.num() == 1 && prod.den() == 6);

    Rational quot = r1.div(r2);
    assert(quot.num() == 3 && quot.den() == 2);

    // Нормализация знака и сокращение.
    Rational normalized(-2, -4);
    assert(normalized.num() == 1 && normalized.den() == 2);

    Rational negativeDen(1, -4);
    assert(negativeDen.num() == -1 && negativeDen.den() == 4);

    std::cout << "06.02: All tests passed.\n";

    // Демонстрационный пример.
    Rational r3(5, 6);
    Rational r4(7, 8);
    Rational r5 = r3.mul(r4);
    std::cout << "Example: (5/6) * (7/8) = "
              << r5.num() << "/" << r5.den() << "\n";

    return 0;
}
