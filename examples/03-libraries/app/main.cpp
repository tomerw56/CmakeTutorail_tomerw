#include <windows.h>
#include <iostream>

using AddFn = int(*)(int, int);
using MultiplyFn = int(*)(int, int);

int main()
{
    HMODULE dll = LoadLibraryA("math_dll.dll");
    if (!dll)
    {
        std::cerr << "Failed to load math_dll.dll\n";
        return 1;
    }

    auto add_fn = reinterpret_cast<AddFn>(GetProcAddress(dll, "add"));
    auto multiply_fn = reinterpret_cast<MultiplyFn>(GetProcAddress(dll, "multiply"));

    if (!add_fn)
    {
        std::cerr << "Failed to find exported function: add\n";
        FreeLibrary(dll);
        return 1;
    }

    if (!multiply_fn)
    {
        std::cerr << "Failed to find exported function: multiply\n";
        FreeLibrary(dll);
        return 1;
    }

    std::cout << "2 + 3 = " << add_fn(2, 3) << "\n";
    std::cout << "4 * 5 = " << multiply_fn(4, 5) << "\n";

    FreeLibrary(dll);
    return 0;
}