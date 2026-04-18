#include "core_math/math_api.h"
#include "internal_math_helpers.h"

namespace core_math
{
    int add(int a, int b)
    {
#ifdef CORE_MATH_INTERNAL_BUILD
        return checked_add_impl(a, b);
#else
        return a + b;
#endif
    }

    int multiply(int a, int b)
    {
#ifdef CORE_MATH_INTERNAL_BUILD
        return checked_multiply_impl(a, b);
#else
        return a * b;
#endif
    }
}