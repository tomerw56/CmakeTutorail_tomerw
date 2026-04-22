#include <iostream>
#include "generated_version.h"
#include "demo_api.h"

int main()
{
    std::cout << "Application version: " << generated_version::version() << "\n";
    std::cout << "Major: " << generated_version::major() << "\n";
    std::cout << "Minor: " << generated_version::minor() << "\n";
    std::cout << "Patch: " << generated_version::patch() << "\n";
    std::cout << "DLL name: " << demo_dll_name() << "\n";

    return 0;
}