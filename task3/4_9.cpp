//////////////////////////////////////////////////////////////////

#include <cstddef>
#include <type_traits> // ТОЛЬКО для тестов static_assert

//////////////////////////////////////////////////////////////////
// integral_constant, true_type, false_type
//////////////////////////////////////////////////////////////////

template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept { return value; }
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <bool B>
using bool_constant = integral_constant<bool, B>;

//////////////////////////////////////////////////////////////////
// is_class (без std::is_union)
//////////////////////////////////////////////////////////////////

template <typename T>
struct is_class
{
private:
    template <typename U>
    static true_type test(int U::*);

    template <typename U>
    static false_type test(...);

public:
    static constexpr bool value =
        decltype(test<T>(0))::value && !__is_union(T);
};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;


//////////////////////////////////////////////////////////////////
// add_const
//////////////////////////////////////////////////////////////////

template <typename T>
struct add_const
{
    using type = const T;
};

template <typename T>
using add_const_t = typename add_const<T>::type;

//////////////////////////////////////////////////////////////////
// remove_const
//////////////////////////////////////////////////////////////////

template <typename T>
struct remove_const
{
    using type = T;
};

template <typename T>
struct remove_const<const T>
{
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

//////////////////////////////////////////////////////////////////
// remove_reference
//////////////////////////////////////////////////////////////////

template <typename T>
struct remove_reference
{
    using type = T;
};

template <typename T>
struct remove_reference<T&>
{
    using type = T;
};

template <typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

//////////////////////////////////////////////////////////////////
// remove_extent (для decay)
//////////////////////////////////////////////////////////////////

template <typename T>
struct remove_extent
{
    using type = T;
};

template <typename T, std::size_t N>
struct remove_extent<T[N]>
{
    using type = T;
};

template <typename T>
struct remove_extent<T[]>
{
    using type = T;
};

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

//////////////////////////////////////////////////////////////////
// is_array
//////////////////////////////////////////////////////////////////

template <typename T>
struct is_array : false_type {};

template <typename T, std::size_t N>
struct is_array<T[N]> : true_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

//////////////////////////////////////////////////////////////////
// is_function
//////////////////////////////////////////////////////////////////

template <typename T>
struct is_function : false_type {};

template <typename R, typename... Args>
struct is_function<R(Args...)> : true_type {};

template <typename R, typename... Args>
struct is_function<R(Args..., ...)> : true_type {};

template <typename T>
inline constexpr bool is_function_v = is_function<T>::value;

//////////////////////////////////////////////////////////////////
// conditional
//////////////////////////////////////////////////////////////////

template <bool B, typename T, typename F>
struct conditional
{
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F>
{
    using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

//////////////////////////////////////////////////////////////////
// decay
//////////////////////////////////////////////////////////////////

template <typename T>
struct decay
{
private:
    using U = remove_reference_t<T>;

public:
    using type =
        conditional_t<
            is_array_v<U>,
            remove_extent_t<U>*,
            conditional_t<
                is_function_v<U>,
                U*,
                remove_const_t<U>
            >
        >;
};

template <typename T>
using decay_t = typename decay<T>::type;

//////////////////////////////////////////////////////////////////
// TESTS
//////////////////////////////////////////////////////////////////

struct MyClass {};
union MyUnion {};

static_assert(is_class_v<MyClass>);
static_assert(!is_class_v<int>);
static_assert(!is_class_v<MyUnion>);

static_assert(std::is_same_v<add_const_t<int>, const int>);
static_assert(std::is_same_v<remove_const_t<const int>, int>);

static_assert(std::is_same_v<decay_t<int>, int>);
static_assert(std::is_same_v<decay_t<const int>, int>);
static_assert(std::is_same_v<decay_t<int&>, int>);
static_assert(std::is_same_v<decay_t<int[3]>, int*>);

using fn = int(double);
static_assert(std::is_same_v<decay_t<fn>, fn*>);

//////////////////////////////////////////////////////////////////

int main() {}
