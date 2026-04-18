#include <iostream>

#include "core_math/math_api.h"
#include "math_headers/build_info.h"

int main()
{
    std::cout << "Build info: " << math_headers::build_mode_name() << "\n";
    std::cout << "Core math API level: " << CORE_MATH_API_LEVEL << "\n";

    std::cout << "10 + 5 = " << core_math::add(10, 5) << "\n";
    std::cout << "10 * 5 = " << core_math::multiply(10, 5) << "\n";

#ifdef MATH_HEADERS_ENABLED
    std::cout << "Header-only interface requirements propagated correctly.\n";
#else
    std::cout << "Header-only interface requirements missing.\n";
#endif

    return 0;
}