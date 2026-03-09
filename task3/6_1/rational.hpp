#pragma once

#include <numeric>

class Rational {
public:
    Rational(int n = 0, int d = 1) : num(n), den(d) {
        simplify();
    }

    int getNum() const { return num; }
    int getDen() const { return den; }

    Rational add(const Rational& other) const;
    Rational sub(const Rational& other) const;
    Rational mul(const Rational& other) const;
    Rational div(const Rational& other) const;

private:
    int num;
    int den;

    void simplify() {
        if (den == 0) throw "Denominator cannot be zero";
        int common = std::gcd(num, den);
        num /= common;
        den /= common;
        if (den < 0) {
            num = -num;
            den = -den;
        }
    }
};