#include <boost/dll/alias.hpp>
#include <string>

std::string test_v1()
{
    return "Plugin v1: algorithm A is active";
}

BOOST_DLL_ALIAS(test_v1, test)
