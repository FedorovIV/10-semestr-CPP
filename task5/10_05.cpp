#include <iostream>
#include <string>
#include <vector>

std::string longest_common_substring(std::string const& lhs, std::string const& rhs)
{
    std::vector<std::vector<std::size_t>> dp(
        lhs.size() + 1,
        std::vector<std::size_t>(rhs.size() + 1, 0));

    std::size_t best_length = 0;
    std::size_t best_end = 0;

    for (std::size_t i = 1; i <= lhs.size(); ++i)
    {
        for (std::size_t j = 1; j <= rhs.size(); ++j)
        {
            if (lhs[i - 1] == rhs[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > best_length)
                {
                    best_length = dp[i][j];
                    best_end = i;
                }
            }
        }
    }

    return lhs.substr(best_end - best_length, best_length);
}

int main()
{
    if (longest_common_substring("blueberry", "blackberry") != "berry")
    {
        std::cerr << "Unexpected longest common substring result\n";
        return 1;
    }
    if (!longest_common_substring("abc", "xyz").empty())
    {
        std::cerr << "Expected empty substring\n";
        return 2;
    }

    std::cout << "10.05: " << longest_common_substring("mississippi", "issip") << '\n';
    return 0;
}
