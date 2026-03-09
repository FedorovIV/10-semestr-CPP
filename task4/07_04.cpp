#include <algorithm>
#include <cstddef>
#include <random>
#include <string>
#include <vector>

#include <gtest/gtest.h>

template <typename T>
void insertion_sort(std::vector<T>& values, std::size_t left, std::size_t right)
{
    for (std::size_t i = left + 1; i < right; ++i)
    {
        for (std::size_t j = i; j > left && values[j] < values[j - 1]; --j)
        {
            std::swap(values[j], values[j - 1]);
        }
    }
}

template <typename T>
std::size_t choose_pivot(std::vector<T>& values, std::size_t left, std::size_t right)
{
    std::size_t const mid = left + (right - left) / 2;
    std::size_t const last = right - 1;

    if (values[mid] < values[left])
    {
        std::swap(values[mid], values[left]);
    }
    if (values[last] < values[left])
    {
        std::swap(values[last], values[left]);
    }
    if (values[last] < values[mid])
    {
        std::swap(values[last], values[mid]);
    }

    return mid;
}

template <typename T>
std::size_t hoare_partition(std::vector<T>& values, std::size_t left, std::size_t right)
{
    T const pivot = values[choose_pivot(values, left, right)];
    std::size_t i = left;
    std::size_t j = right - 1;

    while (true)
    {
        while (values[i] < pivot)
        {
            ++i;
        }
        while (pivot < values[j])
        {
            --j;
        }
        if (i >= j)
        {
            return j;
        }
        std::swap(values[i], values[j]);
        ++i;
        --j;
    }
}

template <typename T>
void quick_sort_impl(std::vector<T>& values, std::size_t left, std::size_t right)
{
    constexpr std::size_t threshold = 16;
    if (right - left <= 1)
    {
        return;
    }
    if (right - left <= threshold)
    {
        insertion_sort(values, left, right);
        return;
    }

    std::size_t const split = hoare_partition(values, left, right);
    quick_sort_impl(values, left, split + 1);
    quick_sort_impl(values, split + 1, right);
}

template <typename T>
void quick_sort(std::vector<T>& values)
{
    if (!values.empty())
    {
        quick_sort_impl(values, 0, values.size());
    }
}

TEST(QuickSort, SortsIntegers)
{
    std::vector<int> values = {5, 4, 3, 2, 1};
    quick_sort(values);
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

TEST(QuickSort, SortsDoubles)
{
    std::vector<double> values = {3.5, -1.2, 3.5, 8.0, 0.0};
    quick_sort(values);
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

TEST(QuickSort, SortsStrings)
{
    std::vector<std::string> values = {"pear", "apple", "orange", "banana"};
    quick_sort(values);
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

TEST(QuickSort, MatchesStdSortOnRandomData)
{
    std::mt19937 engine(12345);
    std::uniform_int_distribution<int> distribution(-100'000, 100'000);

    std::vector<int> values(10'000);
    for (int& value : values)
    {
        value = distribution(engine);
    }

    std::vector<int> expected = values;
    quick_sort(values);
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(values, expected);
}

TEST(QuickSort, HandlesAlreadySortedInput)
{
    std::vector<int> values(2'000);
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        values[i] = static_cast<int>(i);
    }
    quick_sort(values);
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
