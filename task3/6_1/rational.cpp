#include "rational.hpp"

Rational Rational::add(const Rational& other) const {
    return Rational(num * other.den + other.num * den, den * other.den);
}

Rational Rational::sub(const Rational& other) const {
    return Rational(num * other.den - other.num * den, den * other.den);
}

Rational Rational::mul(const Rational& other) const {
    return Rational(num * other.num, den * other.den);
}

Rational Rational::div(const Rational& other) const {
    return Rational(num * other.den, den * other.num);
}