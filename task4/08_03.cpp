#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>

int floor_log2_int(int value)
{
    if (value <= 0)
    {
        throw std::invalid_argument("floor_log2_int: value must be positive");
    }

    unsigned int bits = static_cast<unsigned int>(value);
    int result = -1;
    while (bits != 0U)
    {
        bits >>= 1U;
        ++result;
    }
    return result;
}

int floor_log2_float(float value)
{
    if (!(value > 0.0F))
    {
        throw std::invalid_argument("floor_log2_float: value must be positive");
    }

    union FloatBits
    {
        float f;
        unsigned int u;
    };

    FloatBits bits{};
    bits.f = value;

    unsigned int const exponent = (bits.u >> 23U) & 0xFFU;
    unsigned int mantissa = bits.u & 0x7FFFFFU;

    if (exponent == 0xFFU)
    {
        throw std::domain_error("floor_log2_float: inf and nan are not supported");
    }

    if (exponent != 0U)
    {
        return static_cast<int>(exponent) - 127;
    }

    // Subnormal numbers: value = mantissa * 2^-149.
    int highest_bit = -1;
    while (mantissa != 0U)
    {
        mantissa >>= 1U;
        ++highest_bit;
    }
    if (highest_bit < 0)
    {
        throw std::invalid_argument("floor_log2_float: zero is not supported");
    }
    return highest_bit - 149;
}

int main()
{
    assert(floor_log2_int(1) == 0);
    assert(floor_log2_int(2) == 1);
    assert(floor_log2_int(3) == 1);
    assert(floor_log2_int(1024) == 10);

    assert(floor_log2_float(1.0F) == 0);
    assert(floor_log2_float(2.0F) == 1);
    assert(floor_log2_float(3.5F) == 1);
    assert(floor_log2_float(0.75F) == -1);

    if (floor_log2_float(std::numeric_limits<float>::denorm_min()) != -149)
    {
        std::cerr << "Unexpected log2 for denorm_min\n";
        return 1;
    }

    bool caught = false;
    try
    {
        (void)floor_log2_float(std::numeric_limits<float>::infinity());
    }
    catch (std::domain_error const&)
    {
        caught = true;
    }
    if (!caught)
    {
        std::cerr << "Expected domain_error for infinity\n";
        return 2;
    }

    std::cout << "08.03: all tests passed\n";
    return 0;
}
