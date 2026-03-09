export module rational;

import <numeric>;
import <stdexcept>;

export namespace math {

class Rational {
public:
    Rational(int n = 0, int d = 1);

    int num() const noexcept;
    int den() const noexcept;

    Rational add(const Rational& other) const;
    Rational sub(const Rational& other) const;
    Rational mul(const Rational& other) const;
    Rational div(const Rational& other) const;

private:
    int num_;
    int den_;

    void simplify();
};

}  // namespace math
