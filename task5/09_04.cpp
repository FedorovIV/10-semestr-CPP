#include <deque>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template <typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last)
{
    if (first == last)
    {
        return;
    }

    for (RandomIt current = std::next(first); current != last; ++current)
    {
        RandomIt scan = current;
        while (scan != first)
        {
            RandomIt previous = std::prev(scan);
            if (!(*scan < *previous))
            {
                break;
            }
            std::iter_swap(scan, previous);
            scan = previous;
        }
    }
}

template <typename RandomIt>
RandomIt choose_pivot(RandomIt first, RandomIt last)
{
    RandomIt middle = first;
    std::advance(middle, std::distance(first, last) / 2);
    RandomIt tail = std::prev(last);

    if (*middle < *first)
    {
        std::iter_swap(middle, first);
    }
    if (*tail < *first)
    {
        std::iter_swap(tail, first);
    }
    if (*tail < *middle)
    {
        std::iter_swap(tail, middle);
    }

    return middle;
}

template <typename RandomIt>
RandomIt hoare_partition(RandomIt first, RandomIt last)
{
    auto const pivot = *choose_pivot(first, last);
    RandomIt left = first;
    RandomIt right = std::prev(last);

    while (true)
    {
        while (*left < pivot)
        {
            ++left;
        }
        while (pivot < *right)
        {
            --right;
        }
        if (!(left < right))
        {
            return right;
        }
        std::iter_swap(left, right);
        ++left;
        --right;
    }
}

template <typename RandomIt>
void sort(RandomIt first, RandomIt last)
{
    auto const size = std::distance(first, last);
    if (size <= 1)
    {
        return;
    }
    if (size <= 16)
    {
        insertion_sort(first, last);
        return;
    }

    RandomIt middle = hoare_partition(first, last);
    sort(first, std::next(middle));
    sort(std::next(middle), last);
}

template <typename Container>
bool is_sorted(Container const& container)
{
    for (auto current = std::next(container.begin()); current != container.end(); ++current)
    {
        if (*current < *std::prev(current))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    std::vector<int> numbers = {5, 4, 3, 2, 1};
    sort(numbers.begin(), numbers.end());
    if (!is_sorted(numbers))
    {
        std::cerr << "Vector sort failed\n";
        return 1;
    }

    std::deque<int> deque = {9, 1, 7, 3, 5, 2};
    sort(deque.begin(), deque.end());
    if (!is_sorted(deque))
    {
        std::cerr << "Deque sort failed\n";
        return 2;
    }

    std::vector<std::string> words = {"pear", "apple", "orange", "banana"};
    sort(words.begin(), words.end());
    if (!is_sorted(words))
    {
        std::cerr << "String sort failed\n";
        return 3;
    }

    std::cout << "09.04: iterator-based sort works\n";
    return 0;
}
