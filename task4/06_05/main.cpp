#include <cassert>
#include <iostream>
#include <string>

#include <boost/dll/import.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>

std::string default_library_name_v1()
{
#if defined(_WIN32)
    return "task4_plugin_v1.dll";
#elif defined(__APPLE__)
    return "libtask4_plugin_v1.dylib";
#else
    return "libtask4_plugin_v1.so";
#endif
}

std::string default_library_name_v2()
{
#if defined(_WIN32)
    return "task4_plugin_v2.dll";
#elif defined(__APPLE__)
    return "libtask4_plugin_v2.dylib";
#else
    return "libtask4_plugin_v2.so";
#endif
}

std::string run_from_library(std::string const& file_name)
{
    boost::filesystem::path path(file_name);
    if (path.is_relative())
    {
        path = boost::dll::program_location().parent_path() / path;
    }

    boost::function<std::string()> test_function =
        boost::dll::import_alias<std::string()>(path, "test");

    return test_function();
}

void self_test_if_possible()
{
    boost::filesystem::path const base = boost::dll::program_location().parent_path();
    boost::filesystem::path const v1_path = base / default_library_name_v1();
    boost::filesystem::path const v2_path = base / default_library_name_v2();

    if (boost::filesystem::exists(v1_path) && boost::filesystem::exists(v2_path))
    {
        std::string const r1 = run_from_library(v1_path.string());
        std::string const r2 = run_from_library(v2_path.string());
        assert(r1.find("v1") != std::string::npos);
        assert(r2.find("v2") != std::string::npos);
    }
}

int main()
{
    try
    {
        self_test_if_possible();
    }
    catch (std::exception const& error)
    {
        std::cerr << "Self-test failed: " << error.what() << '\n';
        return 1;
    }

    std::cout << "Enter dynamic library file name: ";
    std::string file_name;
    if (!(std::cin >> file_name))
    {
        std::cerr << "Input error\n";
        return 2;
    }

    try
    {
        std::string const message = run_from_library(file_name);
        std::cout << "Loaded message: " << message << '\n';
    }
    catch (std::exception const& error)
    {
        std::cerr << "Import failed: " << error.what() << '\n';
        return 3;
    }

    return 0;
}
