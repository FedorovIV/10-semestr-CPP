////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include <random>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////
// Сортировка вставками (для маленьких участков)
////////////////////////////////////////////////////////////////////////////////////

void order(std::vector<int> &v, std::size_t left, std::size_t right)
{
    for (auto i = left + 1; i < right; ++i)
    {
        for (auto j = i; j > left; --j)
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

std::size_t pivot_index(std::vector<int> &v, std::size_t left, std::size_t right)
{
    auto mid = left + (right - left) / 2;

    if (v[mid] < v[left])
        std::swap(v[left], v[mid]);
    if (v[right - 1] < v[left])
        std::swap(v[left], v[right - 1]);
    if (v[right - 1] < v[mid])
        std::swap(v[mid], v[right - 1]);

    return mid;
}

////////////////////////////////////////////////////////////////////////////////////
// Разбиение Хоара
////////////////////////////////////////////////////////////////////////////////////

std::size_t partition(std::vector<int> &v, std::size_t left, std::size_t right)
{
    auto p = pivot_index(v, left, right);
    auto pivot = v[p];

    std::size_t i = left - 1;
    std::size_t j = right;

    for (;;)
    {
        do
            ++i;
        while (v[i] < pivot);
        do
            --j;
        while (v[j] > pivot);

        if (i >= j)
            return j;

        std::swap(v[i], v[j]);
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Быстрая сортировка + вставками при малом размере
////////////////////////////////////////////////////////////////////////////////////

void quick(std::vector<int> &v, std::size_t left, std::size_t right)
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

void sort(std::vector<int> &v)
{
    quick(v, 0, v.size());
}

////////////////////////////////////////////////////////////////////////////////////
// ТЕСТЫ
////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // ----------------------------------------------------------
    // test 1: empty
    {
        std::vector<int> v;
        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // test 2: one element
    {
        std::vector<int> v = {42};
        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // test 3: all equal
    {
        std::vector<int> v(100, 7);
        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // test 4: already sorted
    {
        std::vector<int> v(100);
        for (std::size_t i = 0; i < v.size(); ++i)
            v[i] = i;

        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // test 5: reversed
    {
        std::vector<int> v(500);
        for (std::size_t i = 0; i < v.size(); ++i)
            v[i] = v.size() - i;

        sort(v);
        assert(std::ranges::is_sorted(v));
    }

    // test 6: random input
    {
        std::vector<int> v(1000);
        std::mt19937 gen(123);
        std::uniform_int_distribution<int> dist(0, 10000);

        for (auto &x : v)
            x = dist(gen);

        auto copy = v;

        sort(v);
        std::ranges::sort(copy);

        assert(v == copy);
    }

    // test 7: large reversed
    {
        constexpr std::size_t size = 5000;

        std::vector<int> v(size);

        for (std::size_t i = 0; i < size; ++i)
            v[i] = size - i;

        sort(v);

        assert(std::ranges::is_sorted(v));
    }

    // ----------------------------------------------------------

    std::cout << "All tests passed!\n";

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
