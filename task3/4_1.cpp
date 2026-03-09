////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include <random>
#include <string>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////
// Сортировка вставками (для маленьких участков)
////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void order(std::vector<T> & v, std::size_t left, std::size_t right)
{
    for (std::size_t i = left + 1; i < right; ++i)
    {
        for (std::size_t j = i; j > left; --j)
        {
            if (v[j - 1] > v[j])
            {
                std::swap(v[j - 1], v[j]);
            }
            else
                break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Медиана трёх
////////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::size_t pivot_index(std::vector<T> & v, std::size_t left, std::size_t right)
{
    auto mid = left + (right - left) / 2;

    if (v[mid] < v[left])        std::swap(v[left], v[mid]);
    if (v[right - 1] < v[left]) std::swap(v[left], v[right - 1]);
    if (v[right - 1] < v[mid])  std::swap(v[mid], v[right - 1]);

    return mid;
}

////////////////////////////////////////////////////////////////////////////////////
// Разбиение Хоара
////////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::size_t partition(std::vector<T> & v, std::size_t left, std::size_t right)
{
    auto p = pivot_index(v, left, right);
    auto pivot = v[p];

    std::size_t i = left - 1;
    std::size_t j = right;

    for (;;)
    {
        do { ++i; } while (v[i] < pivot);
        do { --j; } while (v[j] > pivot);

        if (i >= j)
            return j;

        std::swap(v[i], v[j]);
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Быстрая сортировка + вставки при малом размере
////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void quick(std::vector<T> & v, std::size_t left, std::size_t right)
{
    if (right - left <= 16)
    {
        order(v, left, right);
        return;
    }

    auto p = partition(v, left, right);

    quick(v, left, p + 1);
    quick(v, p + 1, right);
}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void sort(std::vector<T> & v)
{
    if (!v.empty())
        quick(v, 0, v.size());
}

////////////////////////////////////////////////////////////////////////////////////
// ТЕСТЫ
////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // ----------------------------------------------------------
    // int
    {
        std::vector<int> v = {5, 4, 3, 2, 1};
        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // double
    {
        std::vector<double> v = {3.14, 2.71, 1.41, 0.57};
        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // string
    {
        std::vector<std::string> v = {
            "pear", "apple", "orange", "banana"
        };
        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // random int + compare with std::sort
    {
        std::vector<int> v(1000);
        std::mt19937 gen(123);
        std::uniform_int_distribution<int> dist(0, 10000);

        for (auto & x : v)
            x = dist(gen);

        auto copy = v;

        sort(v);
        std::ranges::sort(copy);

        assert(v == copy);
    }

    // large reversed
    {
        constexpr std::size_t size = 5000;
        std::vector<int> v(size);

        for (std::size_t i = 0; i < size; ++i)
            v[i] = size - i;

        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // ----------------------------------------------------------

    std::cout << "All generic tests passed!\n";
}

////////////////////////////////////////////////////////////////////////////////////
