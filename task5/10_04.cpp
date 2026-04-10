#include <iostream>
#include <vector>

using Grid = std::vector<std::vector<int>>;

int live_neighbors(Grid const& grid, int row, int column)
{
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }
            int const nr = row + dr;
            int const nc = column + dc;
            if (nr >= 0 && nr < static_cast<int>(grid.size()) &&
                nc >= 0 && nc < static_cast<int>(grid[nr].size()))
            {
                count += grid[nr][nc];
            }
        }
    }
    return count;
}

Grid next_generation(Grid const& grid)
{
    Grid next = grid;
    for (int row = 0; row < static_cast<int>(grid.size()); ++row)
    {
        for (int column = 0; column < static_cast<int>(grid[row].size()); ++column)
        {
            int const neighbors = live_neighbors(grid, row, column);
            if (grid[row][column] == 1)
            {
                next[row][column] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            }
            else
            {
                next[row][column] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
    return next;
}

void print_grid(Grid const& grid, int step)
{
    std::cout << "step " << step << '\n';
    for (auto const& row : grid)
    {
        for (int cell : row)
        {
            std::cout << (cell == 1 ? '#' : '.');
        }
        std::cout << '\n';
    }
}

int main()
{
    Grid grid(10, std::vector<int>(10, 0));
    grid[4][3] = 1;
    grid[4][4] = 1;
    grid[4][5] = 1;

    Grid const original = grid;
    print_grid(grid, 0);

    for (int step = 1; step <= 4; ++step)
    {
        grid = next_generation(grid);
        print_grid(grid, step);
    }

    Grid const restored = next_generation(next_generation(original));
    if (restored != original)
    {
        std::cerr << "Blinker oscillator check failed\n";
        return 1;
    }

    std::cout << "10.04: Game of Life simulation completed\n";
    return 0;
}
