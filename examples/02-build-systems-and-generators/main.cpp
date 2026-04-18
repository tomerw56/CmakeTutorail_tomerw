#include <cassert>
#include <iostream>

int main()
{
#ifndef NDEBUG
    std::cout << "Debug-style build\n";
#else
    std::cout << "Release-style build\n";
#endif

    assert(true);
    return 0;
}