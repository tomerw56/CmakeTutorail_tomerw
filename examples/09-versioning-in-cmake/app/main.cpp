#include <iostream>
#include "generated_version.h"

int main()
{
    std::cout << "Application version: " << generated_version::version() << "\n";
    std::cout << "Major: " << generated_version::major() << "\n";
    std::cout << "Minor: " << generated_version::minor() << "\n";
    std::cout << "Patch: " << generated_version::patch() << "\n";

    return 0;
}