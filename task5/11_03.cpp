#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template <typename RandomIt, typename Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare compare)
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
            if (!compare(*scan, *previous))
            {
                break;
            }
            std::iter_swap(scan, previous);
            scan = previous;
        }
    }
}

template <typename RandomIt, typename Compare>
RandomIt choose_pivot(RandomIt first, RandomIt last, Compare compare)
{
    auto const size = std::distance(first, last);
    RandomIt middle = first + size / 2;
    RandomIt tail = std::prev(last);

    if (compare(*middle, *first))
    {
        std::iter_swap(middle, first);
    }
    if (compare(*tail, *first))
    {
        std::iter_swap(tail, first);
    }
    if (compare(*tail, *middle))
    {
        std::iter_swap(tail, middle);
    }

    return middle;
}

template <typename RandomIt, typename Compare>
RandomIt hoare_partition(RandomIt first, RandomIt last, Compare compare)
{
    auto const pivot = *choose_pivot(first, last, compare);
    RandomIt left = first;
    RandomIt right = std::prev(last);

    while (true)
    {
        // Hoare partition moves both iterators until they stop on misplaced elements.
        while (compare(*left, pivot))
        {
            ++left;
        }
        while (compare(pivot, *right))
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

template <typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare compare)
{
    auto const size = std::distance(first, last);
    if (size <= 1)
    {
        return;
    }
    if (size <= 16)
    {
        insertion_sort(first, last, compare);
        return;
    }

    RandomIt middle = hoare_partition(first, last, compare);
    sort(first, std::next(middle), compare);
    sort(std::next(middle), last, compare);
}

template <typename Container, typename Compare>
bool is_sorted_by(Container const& container, Compare compare)
{
    for (auto current = std::next(container.begin()); current != container.end(); ++current)
    {
        if (compare(*current, *std::prev(current)))
        {
            return false;
        }
    }
    return true;
}

bool descending_order(int lhs, int rhs)
{
    return lhs > rhs;
}

int main()
{
    std::vector<int> values = {5, 4, 3, 2, 1, 9, 7};
    sort(values.begin(), values.end(), descending_order);
    if (!is_sorted_by(values, descending_order))
    {
        std::cerr << "Free-function comparator failed\n";
        return 1;
    }

    std::deque<int> deque = {9, 1, 7, 3, 5, 2};
    sort(deque.begin(), deque.end(), std::less<>{});
    if (!is_sorted_by(deque, std::less<>{}))
    {
        std::cerr << "std::less comparator failed\n";
        return 2;
    }

    auto by_length = [](std::string const& lhs, std::string const& rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return lhs.size() < rhs.size();
        }
        return lhs < rhs;
    };

    std::vector<std::string> words = {"pear", "apple", "kiwi", "banana", "fig"};
    sort(words.begin(), words.end(), by_length);
    if (!is_sorted_by(words, by_length))
    {
        std::cerr << "Lambda comparator failed\n";
        return 3;
    }

    std::cout << "11.03: free function comparator ->";
    for (int value : values)
    {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    std::cout << "11.03: std::less comparator ->";
    for (int value : deque)
    {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    std::cout << "11.03: lambda comparator ->";
    for (std::string const& word : words)
    {
        std::cout << ' ' << word;
    }
    std::cout << '\n';
    return 0;
}

// 11-03 is about comparison function 
// Need to resubmit
