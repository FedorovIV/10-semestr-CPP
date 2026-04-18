#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

struct TspResult
{
    std::vector<int> route;
    int cost = std::numeric_limits<int>::max();
};

TspResult solve_tsp(std::vector<std::vector<int>> const& graph)
{
    int const size = static_cast<int>(graph.size());
    std::vector<int> order;
    for (int vertex = 1; vertex < size; ++vertex)
    {
        order.push_back(vertex);
    }

    TspResult best;
    do
    {
        int cost = 0;
        int previous = 0;
        for (int vertex : order)
        {
            cost += graph[previous][vertex];
            previous = vertex;
        }
        cost += graph[previous][0];

        if (cost < best.cost)
        {
            best.cost = cost;
            best.route = {0};
            best.route.insert(best.route.end(), order.begin(), order.end());
            best.route.push_back(0);
        }
    }
    while (std::next_permutation(order.begin(), order.end()));

    return best;
}

std::vector<std::vector<int>> random_graph(int size)
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<int> distribution(1, 10);

    std::vector<std::vector<int>> graph(size, std::vector<int>(size, 0));
    for (int row = 0; row < size; ++row)
    {
        for (int column = row + 1; column < size; ++column)
        {
            int const weight = distribution(engine);
            graph[row][column] = weight;
            graph[column][row] = weight;
        }
    }
    return graph;
}

void print_graph(std::vector<std::vector<int>> const& graph)
{
    std::cout << "graph:\n";
    for (auto const& row : graph)
    {
        for (int weight : row)
        {
            std::cout << weight << ' ';
        }
        std::cout << '\n';
    }
}

int main()
{
    std::vector<std::vector<int>> const known = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    TspResult const known_result = solve_tsp(known);
    if (known_result.cost != 80)
    {
        std::cerr << "Known TSP instance failed\n";
        return 1;
    }

    std::vector<std::vector<int>> const graph = random_graph(10);
    TspResult const result = solve_tsp(graph);
    if (result.route.size() != 11 || result.cost <= 0)
    {
        std::cerr << "Unexpected TSP result\n";
        return 2;
    }

    print_graph(graph);
    std::cout << "best route:";
    for (int vertex : result.route)
    {
        std::cout << ' ' << vertex;
    }
    std::cout << '\n';
    std::cout << "best cost: " << result.cost << '\n';
    return 0;
}

// 11-03 is about comparison function 
// Need to resubmit
