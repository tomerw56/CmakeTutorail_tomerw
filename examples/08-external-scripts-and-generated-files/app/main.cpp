#include <iostream>
#include "generated_build_info.h"

int main()
{
    std::cout << "Generated build label: " << generated_build_info::label() << "\n";
    return 0;
}