#include <algorithm>
#include <cstddef>
#include <vector>

#include <benchmark/benchmark.h>

void insertion_sort(std::vector<double>& values, std::size_t left, std::size_t right)
{
    for (std::size_t i = left + 1; i < right; ++i)
    {
        for (std::size_t j = i; j > left && values[j] < values[j - 1]; --j)
        {
            std::swap(values[j], values[j - 1]);
        }
    }
}

std::size_t choose_pivot(std::vector<double>& values, std::size_t left, std::size_t right)
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

std::size_t hoare_partition(std::vector<double>& values, std::size_t left, std::size_t right)
{
    double const pivot = values[choose_pivot(values, left, right)];
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

void hybrid_sort_impl(
    std::vector<double>& values,
    std::size_t left,
    std::size_t right,
    std::size_t threshold)
{
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
    hybrid_sort_impl(values, left, split + 1, threshold);
    hybrid_sort_impl(values, split + 1, right, threshold);
}

void hybrid_sort(std::vector<double>& values, std::size_t threshold)
{
    if (!values.empty())
    {
        hybrid_sort_impl(values, 0, values.size(), threshold);
    }
}

static void BM_HybridSort(benchmark::State& state)
{
    std::size_t const threshold = static_cast<std::size_t>(state.range(0));

    for (auto _ : state)
    {
        std::vector<double> values(10'000);
        for (std::size_t i = 0; i < values.size(); ++i)
        {
            values[i] = static_cast<double>(values.size() - i);
        }

        hybrid_sort(values, threshold);
        if (!std::is_sorted(values.begin(), values.end()))
        {
            state.SkipWithError("hybrid_sort produced unsorted output");
            break;
        }

        benchmark::DoNotOptimize(values.data());
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_HybridSort)->Arg(4)->Arg(8)->Arg(16)->Arg(24)->Arg(32)->Arg(64);

BENCHMARK_MAIN();
