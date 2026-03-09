//////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <numeric>
#include <type_traits>

//////////////////////////////////////////////////////////////////////////////////////////
// RATIO (как std::ratio)
//////////////////////////////////////////////////////////////////////////////////////////

template <int N = 0, int D = 1>
struct Ratio
{
    static_assert(D != 0, "Denominator must not be zero");

private:
    static constexpr int g =
        std::gcd(N, D) == 0 ? 1 : std::gcd(N, D);

    static constexpr int n0 = N / g;
    static constexpr int d0 = D / g;

public:
    static constexpr int num = (d0 < 0 ? -n0 : n0);
    static constexpr int den = (d0 < 0 ? -d0 : d0);

    // 🔑 КАНОНИЧЕСКИЙ ТИП
    using type = Ratio<num, den>;
};

//////////////////////////////////////////////////////////////////////////////////////////
// SUM
//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Sum
{
    using type =
        typename Ratio<
            R1::num * R2::den + R2::num * R1::den,
            R1::den * R2::den
        >::type;
};

template <typename R1, typename R2>
using sum = typename Sum<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////
// SUB (через SUM)
//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Sub
{
    using type = sum<R1, Ratio<-R2::num, R2::den>>;
};

template <typename R1, typename R2>
using sub = typename Sub<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////
// MUL
//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Mul
{
    using type =
        typename Ratio<
            R1::num * R2::num,
            R1::den * R2::den
        >::type;
};

template <typename R1, typename R2>
using mul = typename Mul<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////
// DIV (через MUL)
//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Div
{
    static_assert(R2::num != 0, "Division by zero");

    using type = mul<R1, Ratio<R2::den, R2::num>>;
};

template <typename R1, typename R2>
using div = typename Div<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////
// DURATION
//////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename R = Ratio<1>>
struct Duration
{
    T x{};

    constexpr explicit Duration(T value) : x(value) {}
};

//////////////////////////////////////////////////////////////////////////////////////////
// OPERATOR +
//////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename R1,
          typename T2, typename R2>
constexpr auto operator+(Duration<T1, R1> const& lhs,
                          Duration<T2, R2> const& rhs)
{
    using ratio_t = typename sum<R1, R2>::type;

    auto value =
        lhs.x * ratio_t::den / R1::den * R1::num +
        rhs.x * ratio_t::den / R2::den * R2::num;

    return Duration<decltype(value), ratio_t>(value);
}

//////////////////////////////////////////////////////////////////////////////////////////
// OPERATOR -
//////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename R1,
          typename T2, typename R2>
constexpr auto operator-(Duration<T1, R1> const& lhs,
                          Duration<T2, R2> const& rhs)
{
    return lhs + Duration<T2, Ratio<-R2::num, R2::den>>(rhs.x);
}

//////////////////////////////////////////////////////////////////////////////////////////
// STATIC TESTS (ВАЖНО: сравниваем ::type)
//////////////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same_v<typename Ratio<6,12>::type, Ratio<1,2>>);
static_assert(std::is_same_v<typename Ratio<-1,-2>::type, Ratio<1,2>>);
static_assert(std::is_same_v<typename Ratio<1,-2>::type, Ratio<-1,2>>);

static_assert(std::is_same_v<sum<Ratio<1,2>, Ratio<1,3>>, Ratio<5,6>>);
static_assert(std::is_same_v<sub<Ratio<1,2>, Ratio<1,3>>, Ratio<1,6>>);
static_assert(std::is_same_v<mul<Ratio<2,3>, Ratio<3,4>>, Ratio<1,2>>);
static_assert(std::is_same_v<div<Ratio<1,2>, Ratio<3,4>>, Ratio<2,3>>);

//////////////////////////////////////////////////////////////////////////////////////////
// RUNTIME TEST
//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Duration<int, Ratio<1,2>> d1(1);
    Duration<int, Ratio<1,3>> d2(2);

    auto s = d1 + d2;   // 7/6
    auto d = d1 - d2;   // -1/6

    assert(s.x == 7);
    assert(d.x == -1);
}

//////////////////////////////////////////////////////////////////////////////////////////
