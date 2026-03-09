module rational;

import <numeric>;
import <stdexcept>;

namespace math {

Rational::Rational(int n, int d) : num_(n), den_(d) {
    simplify();
}

int Rational::num() const noexcept {
    return num_;
}

int Rational::den() const noexcept {
    return den_;
}

Rational Rational::add(const Rational& other) const {
    return Rational(num_ * other.den_ + other.num_ * den_, den_ * other.den_);
}

Rational Rational::sub(const Rational& other) const {
    return Rational(num_ * other.den_ - other.num_ * den_, den_ * other.den_);
}

Rational Rational::mul(const Rational& other) const {
    return Rational(num_ * other.num_, den_ * other.den_);
}

Rational Rational::div(const Rational& other) const {
    if (other.num_ == 0) {
        throw std::invalid_argument("division by zero");
    }
    return Rational(num_ * other.den_, den_ * other.num_);
}

void Rational::simplify() {
    if (den_ == 0) {
        throw std::invalid_argument("denominator cannot be zero");
    }

    int common = std::gcd(num_, den_);
    num_ /= common;
    den_ /= common;

    if (den_ < 0) {
        num_ = -num_;
        den_ = -den_;
    }
}

}  // namespace math
