///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <array>


///////////////////////////////////////////////////////////////////////////////////////////////

class IPv4
{
public:
    IPv4(uint8_t a = 0, uint8_t b = 0, uint8_t c = 0, uint8_t d = 0)
        : m_bytes{ a, b, c, d } {}

    // Конструктор из 32-битного числа
    explicit IPv4(uint32_t value)
    {
        m_bytes[0] = (value >> 24) & 0xFF;
        m_bytes[1] = (value >> 16) & 0xFF;
        m_bytes[2] = (value >> 8)  & 0xFF;
        m_bytes[3] = (value)       & 0xFF;
    }

    // Преобразование в uint32_t
    explicit operator uint32_t() const
    {
        return (uint32_t(m_bytes[0]) << 24) |
               (uint32_t(m_bytes[1]) << 16) |
               (uint32_t(m_bytes[2]) << 8)  |
               uint32_t(m_bytes[3]);
    }

    // ------------------------------------------------------------------------
    // Префиксный инкремент
    IPv4& operator++()
    {
        uint32_t value = static_cast<uint32_t>(*this);
        ++value;
        *this = IPv4(value);
        return *this;
    }

    // Постфиксный инкремент
    IPv4 operator++(int)
    {
        IPv4 temp = *this;
        ++(*this);
        return temp;
    }

    // Префиксный декремент
    IPv4& operator--()
    {
        uint32_t value = static_cast<uint32_t>(*this);
        --value;
        *this = IPv4(value);
        return *this;
    }

    // Постфиксный декремент
    IPv4 operator--(int)
    {
        IPv4 temp = *this;
        --(*this);
        return temp;
    }

    // ------------------------------------------------------------------------
    // Операторы сравнения

    friend bool operator==(const IPv4& lhs, const IPv4& rhs)
    {
        return lhs.m_bytes == rhs.m_bytes;
    }

    friend bool operator!=(const IPv4& lhs, const IPv4& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator<(const IPv4& lhs, const IPv4& rhs)
    {
        return static_cast<uint32_t>(lhs) < static_cast<uint32_t>(rhs);
    }

    friend bool operator>(const IPv4& lhs, const IPv4& rhs)
    {
        return rhs < lhs;
    }

    friend bool operator<=(const IPv4& lhs, const IPv4& rhs)
    {
        return !(lhs > rhs);
    }

    friend bool operator>=(const IPv4& lhs, const IPv4& rhs)
    {
        return !(lhs < rhs);
    }

    // ------------------------------------------------------------------------
    // Потоковый ввод
    friend std::istream& operator>>(std::istream& is, IPv4& ip)
    {
        char dot;
        unsigned int bytes[4];

        if (is >> bytes[0] >> dot >> bytes[1] >> dot >> bytes[2] >> dot >> bytes[3])
        {
            for (int i = 0; i < 4; ++i)
                ip.m_bytes[i] = static_cast<uint8_t>(bytes[i]);
        }
        return is;
    }

    // Потоковый вывод
    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip)
    {
        os << static_cast<int>(ip.m_bytes[0]) << '.'
           << static_cast<int>(ip.m_bytes[1]) << '.'
           << static_cast<int>(ip.m_bytes[2]) << '.'
           << static_cast<int>(ip.m_bytes[3]);
        return os;
    }

private:
    std::array<uint8_t, 4> m_bytes;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    IPv4 ip1(192, 168, 0, 1);
    IPv4 ip2(192, 168, 0, 2);

    // Проверка операторов сравнения
    assert(ip1 < ip2);
    assert(ip1 != ip2);
    assert(ip2 > ip1);
    assert(ip1 <= ip2);
    assert(ip2 >= ip1);

    // Проверка инкремента / декремента
    IPv4 ip3 = ip1;
    assert((ip3++) == IPv4(192, 168, 0, 1));
    assert(ip3 == IPv4(192, 168, 0, 2));

    assert((++ip3) == IPv4(192, 168, 0, 3));

    assert((ip3--) == IPv4(192, 168, 0, 3));
    assert(ip3 == IPv4(192, 168, 0, 2));

    assert((--ip3) == IPv4(192, 168, 0, 1));

    // Проверка потокового ввода/вывода
    std::stringstream ss("10.0.0.42");
    IPv4 ip4;
    ss >> ip4;

    std::stringstream ss_out;
    ss_out << ip4;

    assert(ss_out.str() == "10.0.0.42");

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
