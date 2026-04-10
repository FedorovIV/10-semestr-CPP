#include <iostream>
#include <vector>

int main()
{
    std::vector<int> values;
    std::vector<std::size_t> capacities;

    for (int i = 0; i < 256; ++i)
    {
        values.push_back(i);
        if (capacities.empty() || capacities.back() != values.capacity())
        {
            capacities.push_back(values.capacity());
        }
    }

    if (capacities.size() < 3)
    {
        std::cerr << "Not enough capacity changes were observed\n";
        return 1;
    }

    std::cout << "capacities:";
    for (std::size_t capacity : capacities)
    {
        std::cout << ' ' << capacity;
    }
    std::cout << '\n';

    std::vector<double> ratios;
    for (std::size_t i = 1; i < capacities.size(); ++i)
    {
        ratios.push_back(static_cast<double>(capacities[i]) / static_cast<double>(capacities[i - 1]));
    }

    std::cout << "growth ratios:";
    for (double ratio : ratios)
    {
        std::cout << ' ' << ratio;
    }
    std::cout << '\n';

#ifdef __GLIBCXX__
    double const observed = ratios.back();
    if (observed < 1.9 || observed > 2.1)
    {
        std::cerr << "Expected libstdc++ vector growth factor close to 2, got " << observed << '\n';
        return 2;
    }
#endif

    std::cout << "10.01: vector capacity growth tracked successfully\n";
    return 0;
}
