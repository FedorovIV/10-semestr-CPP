//////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <type_traits>

//////////////////////////////////////////////////////////////////
// Empty tuple
//////////////////////////////////////////////////////////////////

template <typename... Ts>
class Tuple
{
public:
    static constexpr std::size_t size()
    {
        return 0;
    }
};

//////////////////////////////////////////////////////////////////
// Non-empty tuple
//////////////////////////////////////////////////////////////////

template <typename T, typename... Ts>
class Tuple<T, Ts...>
{
public:
    Tuple(T&& x, Ts&&... xs)
        : m_head(std::forward<T>(x)),
          m_tail(std::forward<Ts>(xs)...)
    {}

    // ------------------------------------------------------------
    // size()
    // ------------------------------------------------------------

    static constexpr std::size_t size()
    {
        return 1 + sizeof...(Ts);
    }

    // ------------------------------------------------------------
    // get<I>()
    // ------------------------------------------------------------

    template <std::size_t I>
    auto get() const
    {
        if constexpr (I > 0)
        {
            return m_tail.template get<I - 1>();
        }
        else
        {
            return m_head;
        }
    }

private:
    T m_head;
    Tuple<Ts...> m_tail;
};

//////////////////////////////////////////////////////////////////
// STATIC TESTS
//////////////////////////////////////////////////////////////////

static_assert(Tuple<>::size() == 0);
static_assert(Tuple<int>::size() == 1);
static_assert(Tuple<int, double>::size() == 2);
static_assert(Tuple<int, double, std::string>::size() == 3);

//////////////////////////////////////////////////////////////////
// RUNTIME TESTS
//////////////////////////////////////////////////////////////////

int main()
{
    Tuple<int, double, std::string> tuple(1, 2.0, "aaaaa");

    assert(tuple.get<0>() == 1);
    assert(tuple.get<1>() == 2.0);
    assert(tuple.get<2>() == "aaaaa");

    assert(tuple.size() == 3);
}

//////////////////////////////////////////////////////////////////
