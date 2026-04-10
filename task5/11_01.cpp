#include <iostream>

struct Wrapper;
Wrapper test();

struct Wrapper
{
    using function_t = Wrapper (*)();

    operator function_t() const
    {
        return &test;
    }
};

Wrapper test()
{
    static int call_count = 0;
    ++call_count;
    std::cout << "test call " << call_count << '\n';
    return Wrapper{};
}

int main()
{
    Wrapper function = test();
    (*function)();
    std::cout << "11.01: wrapper can call test recursively through conversion\n";
    return 0;
}
