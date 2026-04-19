#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace
{
constexpr std::size_t kStringLength = 10;
constexpr std::size_t kSampleCount = 1u << 20;
constexpr std::size_t kCheckpointStep = 1u << 14;

using Word = std::array<char, kStringLength>;

struct HashFunction
{
    std::string_view name;
    std::uint32_t (*function)(Word const&);
};

struct Measurement
{
    std::string name;
    std::vector<std::size_t> collisions;
    std::size_t final_collisions = 0;
    std::size_t cumulative_collisions = 0;
};

Word make_word(std::string_view text)
{
    if (text.size() != kStringLength)
    {
        throw std::runtime_error("Unexpected word length");
    }

    Word word = {};
    std::copy(text.begin(), text.end(), word.begin());
    return word;
}

std::uint64_t encode_word(Word const& word)
{
    std::uint64_t code = 0;

    for (char character : word)
    {
        code <<= 5u;
        code |= static_cast<std::uint64_t>(character - 'a');
    }

    return code;
}

std::uint32_t rs_hash(Word const& word)
{
    std::uint32_t b = 378551u;
    std::uint32_t a = 63689u;
    std::uint32_t hash = 0u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = hash * a + value;
        a *= b;
    }

    return hash;
}

std::uint32_t js_hash(Word const& word)
{
    std::uint32_t hash = 1315423911u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash ^= ((hash << 5u) + value + (hash >> 2u));
    }

    return hash;
}

std::uint32_t pjw_hash(Word const& word)
{
    std::uint32_t constexpr bits_in_unsigned_int = static_cast<std::uint32_t>(sizeof(std::uint32_t) * 8u);
    std::uint32_t constexpr three_quarters = (bits_in_unsigned_int * 3u) / 4u;
    std::uint32_t constexpr one_eighth = bits_in_unsigned_int / 8u;
    std::uint32_t constexpr high_bits = 0xFFFFFFFFu << (bits_in_unsigned_int - one_eighth);

    std::uint32_t hash = 0u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = (hash << one_eighth) + value;

        std::uint32_t const test = hash & high_bits;
        if (test != 0u)
        {
            hash = (hash ^ (test >> three_quarters)) & (~high_bits);
        }
    }

    return hash;
}

std::uint32_t elf_hash(Word const& word)
{
    std::uint32_t hash = 0u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = (hash << 4u) + value;

        std::uint32_t const high = hash & 0xF0000000u;
        if (high != 0u)
        {
            hash ^= high >> 24u;
        }

        hash &= ~high;
    }

    return hash;
}

std::uint32_t bkdr_hash(Word const& word)
{
    std::uint32_t constexpr seed = 131u;
    std::uint32_t hash = 0u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = hash * seed + value;
    }

    return hash;
}

std::uint32_t sdbm_hash(Word const& word)
{
    std::uint32_t hash = 0u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = value + (hash << 6u) + (hash << 16u) - hash;
    }

    return hash;
}

std::uint32_t djb_hash(Word const& word)
{
    std::uint32_t hash = 5381u;

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = ((hash << 5u) + hash) + value;
    }

    return hash;
}

std::uint32_t dek_hash(Word const& word)
{
    std::uint32_t hash = static_cast<std::uint32_t>(word.size());

    for (char character : word)
    {
        std::uint32_t const value = static_cast<unsigned char>(character);
        hash = ((hash << 5u) ^ (hash >> 27u)) ^ value;
    }

    return hash;
}

std::uint32_t ap_hash(Word const& word)
{
    std::uint32_t hash = 0xAAAAAAAAu;

    for (std::size_t index = 0; index < word.size(); ++index)
    {
        std::uint32_t const value = static_cast<unsigned char>(word[index]);
        if ((index & 1u) == 0u)
        {
            hash ^= ((hash << 7u) ^ (value * (hash >> 3u)));
        }
        else
        {
            hash ^= ~((hash << 11u) + (value ^ (hash >> 5u)));
        }
    }

    return hash;
}

std::vector<HashFunction> const& hash_functions()
{
    static std::vector<HashFunction> const functions = {
        {"RS", &rs_hash},
        {"JS", &js_hash},
        {"PJW", &pjw_hash},
        {"ELF", &elf_hash},
        {"BKDR", &bkdr_hash},
        {"SDBM", &sdbm_hash},
        {"DJB", &djb_hash},
        {"DEK", &dek_hash},
        {"AP", &ap_hash}
    };

    return functions;
}

std::vector<Word> make_unique_words(std::size_t count)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> distribution('a', 'z');

    std::unordered_set<std::uint64_t> used_codes;
    used_codes.reserve(count * 2u);

    std::vector<Word> words;
    words.reserve(count);

    while (words.size() < count)
    {
        Word word = {};
        for (char& character : word)
        {
            character = static_cast<char>(distribution(engine));
        }

        std::uint64_t const code = encode_word(word);
        if (used_codes.insert(code).second)
        {
            words.push_back(word);
        }
    }

    return words;
}

Measurement measure(HashFunction const& function, std::vector<Word> const& words)
{
    std::unordered_set<std::uint32_t> seen_hashes;
    seen_hashes.reserve(words.size());

    Measurement result;
    result.name = std::string(function.name);

    for (std::size_t index = 0; index < words.size(); ++index)
    {
        seen_hashes.insert(function.function(words[index]));

        std::size_t const processed = index + 1u;
        if (processed % kCheckpointStep == 0u || processed == words.size())
        {
            std::size_t const collisions = processed - seen_hashes.size();
            result.collisions.push_back(collisions);
        }
    }

    result.final_collisions = result.collisions.back();
    result.cumulative_collisions = std::accumulate(
        result.collisions.begin(),
        result.collisions.end(),
        static_cast<std::size_t>(0));

    return result;
}

std::vector<std::size_t> make_checkpoints()
{
    std::vector<std::size_t> checkpoints;

    for (std::size_t processed = kCheckpointStep; processed <= kSampleCount; processed += kCheckpointStep)
    {
        checkpoints.push_back(processed);
    }

    if (checkpoints.empty() || checkpoints.back() != kSampleCount)
    {
        checkpoints.push_back(kSampleCount);
    }

    return checkpoints;
}

void write_csv(
    std::filesystem::path const& path,
    std::vector<std::size_t> const& checkpoints,
    std::vector<Measurement> const& measurements)
{
    std::ofstream output(path);
    if (!output)
    {
        throw std::runtime_error("Cannot open CSV output");
    }

    output << "count";
    for (Measurement const& measurement : measurements)
    {
        output << ',' << measurement.name;
    }
    output << '\n';

    for (std::size_t index = 0; index < checkpoints.size(); ++index)
    {
        output << checkpoints[index];
        for (Measurement const& measurement : measurements)
        {
            output << ',' << measurement.collisions[index];
        }
        output << '\n';
    }
}

void write_report(
    std::filesystem::path const& path,
    std::vector<Measurement> const& measurements)
{
    auto const by_quality = [](Measurement const& lhs, Measurement const& rhs)
    {
        if (lhs.final_collisions != rhs.final_collisions)
        {
            return lhs.final_collisions < rhs.final_collisions;
        }
        if (lhs.cumulative_collisions != rhs.cumulative_collisions)
        {
            return lhs.cumulative_collisions < rhs.cumulative_collisions;
        }
        return lhs.name < rhs.name;
    };

    auto const best = std::min_element(measurements.begin(), measurements.end(), by_quality);
    auto const worst = std::max_element(measurements.begin(), measurements.end(), by_quality);

    std::ofstream output(path);
    if (!output)
    {
        throw std::runtime_error("Cannot open report output");
    }

    output << "# 10.05 Results\n\n";
    output << "- Dataset: " << kSampleCount << " unique random strings.\n";
    output << "- String format: fixed length " << kStringLength << ", alphabet a-z.\n";
    output << "- Hash functions: RS, JS, PJW, ELF, BKDR, SDBM, DJB, DEK, AP.\n";
#ifdef TASK5_10_05_M32_ENABLED
    output << "- Build mode: -O3 and -m32.\n";
#else
    output << "- Build mode: -O3. This environment does not provide the 32-bit runtime needed for -m32, ";
    output << "so the fallback build omits it.\n";
#endif
    output << '\n';

    output << "## Final ranking\n\n";
    for (Measurement const& measurement : measurements)
    {
        output << "- " << measurement.name
               << ": final collisions = " << measurement.final_collisions
               << ", cumulative collisions = " << measurement.cumulative_collisions
               << '\n';
    }
    output << '\n';

    output << "## Conclusion\n\n";
    output << "- Best function in this run: " << best->name << ".\n";
    output << "- Worst function in this run: " << worst->name << ".\n";
    output << "- The curves grow slowly at first and then bend upward because collisions in a nearly uniform ";
    output << "32-bit hash space grow approximately like n * (n - 1) / (2 * 2^32).\n";
    output << "- Functions with flatter curves distribute values more evenly on this dataset, while functions ";
    output << "with earlier or steeper growth produce more repeated hash values.\n";
}

void verify_measurements(std::vector<Measurement> const& measurements, std::size_t checkpoint_count)
{
    for (Measurement const& measurement : measurements)
    {
        if (measurement.collisions.size() != checkpoint_count)
        {
            throw std::runtime_error("Unexpected number of checkpoints");
        }

        if (!std::is_sorted(measurement.collisions.begin(), measurement.collisions.end()))
        {
            throw std::runtime_error("Collision counts must be monotonic");
        }
    }
}

void self_test()
{
    Word const word = make_word("aaaaaaaaaa");

    for (HashFunction const& function : hash_functions())
    {
        std::uint32_t const first = function.function(word);
        std::uint32_t const second = function.function(word);
        if (first != second)
        {
            throw std::runtime_error("Hash function is not deterministic");
        }
    }

    std::vector<Word> const test_words = make_unique_words(128);
    std::unordered_set<std::uint64_t> unique_codes;
    for (Word const& element : test_words)
    {
        unique_codes.insert(encode_word(element));
    }

    if (unique_codes.size() != test_words.size())
    {
        throw std::runtime_error("String generator must produce unique strings");
    }
}

std::filesystem::path output_directory(char const* executable_path)
{
    std::filesystem::path const absolute_path = std::filesystem::absolute(executable_path);
    std::filesystem::path const build_directory = absolute_path.parent_path();

    if (build_directory.filename() == "build")
    {
        return build_directory.parent_path();
    }

    return std::filesystem::current_path();
}
} // namespace

int main(int argc, char* argv[])
{
    try
    {
        if (argc <= 0 || argv == nullptr || argv[0] == nullptr)
        {
            std::cerr << "10.05: missing executable path\n";
            return 1;
        }

        self_test();

        std::vector<Word> const words = make_unique_words(kSampleCount);
        std::vector<std::size_t> const checkpoints = make_checkpoints();

        std::vector<Measurement> measurements;
        measurements.reserve(hash_functions().size());

        for (HashFunction const& function : hash_functions())
        {
            measurements.push_back(measure(function, words));
        }

        verify_measurements(measurements, checkpoints.size());
        std::sort(measurements.begin(), measurements.end(), [](Measurement const& lhs, Measurement const& rhs)
        {
            if (lhs.final_collisions != rhs.final_collisions)
            {
                return lhs.final_collisions < rhs.final_collisions;
            }
            if (lhs.cumulative_collisions != rhs.cumulative_collisions)
            {
                return lhs.cumulative_collisions < rhs.cumulative_collisions;
            }
            return lhs.name < rhs.name;
        });

        std::filesystem::path const base_directory = output_directory(argv[0]);
        std::filesystem::path const csv_path = base_directory / "10_05_collisions.csv";
        std::filesystem::path const report_path = base_directory / "10_05_results.md";

        write_csv(csv_path, checkpoints, measurements);
        write_report(report_path, measurements);

        std::cout << "10.05: dataset size = " << kSampleCount << '\n';
        for (Measurement const& measurement : measurements)
        {
            std::cout << "10.05: " << measurement.name
                      << " final collisions = " << measurement.final_collisions
                      << '\n';
        }
        std::cout << "10.05: wrote " << csv_path.filename().string() << '\n';
        std::cout << "10.05: wrote " << report_path.filename().string() << '\n';
        return 0;
    }
    catch (std::exception const& exception)
    {
        std::cerr << "10.05: " << exception.what() << '\n';
        return 2;
    }
}
