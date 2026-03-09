#include <boost/dll/alias.hpp>
#include <string>

std::string test_v2()
{
    return "Plugin v2: algorithm B is active";
}

BOOST_DLL_ALIAS(test_v2, test)
