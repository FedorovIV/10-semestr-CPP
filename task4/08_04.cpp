#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

std::string random_word(
    std::size_t length,
    std::default_random_engine& engine,
    std::uniform_int_distribution<int>& letter_distribution)
{
    std::string result(length, 'a');
    for (char& symbol : result)
    {
        symbol = static_cast<char>('a' + letter_distribution(engine));
    }
    return result;
}

std::string mutate(
    std::string const& parent,
    double mutation_probability,
    std::default_random_engine& engine,
    std::uniform_real_distribution<double>& probability_distribution,
    std::uniform_int_distribution<int>& letter_distribution)
{
    std::string child = parent;
    for (char& symbol : child)
    {
        if (probability_distribution(engine) < mutation_probability)
        {
            symbol = static_cast<char>('a' + letter_distribution(engine));
        }
    }
    return child;
}

std::size_t metric(std::string const& candidate, std::string const& target)
{
    std::size_t mismatch_count = 0;
    for (std::size_t i = 0; i < target.size(); ++i)
    {
        if (candidate[i] != target[i])
        {
            ++mismatch_count;
        }
    }
    return mismatch_count;
}

int main()
{
    std::string const target = "methinksitislikeaweasel";
    double const mutation_probability = 0.05;
    std::size_t const copies_per_generation = 100;
    std::size_t const max_generations = 20'000;

    std::random_device source;
    std::default_random_engine engine(source());
    std::uniform_int_distribution<int> letter_distribution(0, 25);
    std::uniform_real_distribution<double> probability_distribution(0.0, 1.0);

    std::string best = random_word(target.size(), engine, letter_distribution);

    for (std::size_t generation = 0; generation < max_generations; ++generation)
    {
        std::size_t best_metric = metric(best, target);
        std::cout << "gen " << generation
                  << " : " << best
                  << " : mismatch = " << best_metric << '\n';

        if (best_metric == 0)
        {
            std::cout << "08.04: target reached\n";
            return 0;
        }

        std::string generation_best = best;
        std::size_t generation_metric = std::numeric_limits<std::size_t>::max();

        for (std::size_t i = 0; i < copies_per_generation; ++i)
        {
            std::string candidate = mutate(
                best,
                mutation_probability,
                engine,
                probability_distribution,
                letter_distribution);

            std::size_t const candidate_metric = metric(candidate, target);
            if (candidate_metric < generation_metric)
            {
                generation_metric = candidate_metric;
                generation_best = std::move(candidate);
            }
        }

        best = std::move(generation_best);
    }

    std::cerr << "08.04: target was not reached within max_generations\n";
    assert(false && "target should be reached in reasonable number of generations");
    return 1;
}
