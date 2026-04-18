#include <iostream>
#include "math_utils.h"

namespace
{
    bool expect_equal(const char* test_name, int actual, int expected)
    {
        if (actual != expected)
        {
            std::cerr << "[FAIL] " << test_name
                      << " | expected: " << expected
                      << ", actual: " << actual << "\n";
            return false;
        }

        std::cout << "[PASS] " << test_name << "\n";
        return true;
    }
}

int main()
{
    bool ok = true;

    ok = expect_equal("add(2, 3)", add(2, 3), 5) && ok;
    ok = expect_equal("add(-4, 1)", add(-4, 1), -3) && ok;
    ok = expect_equal("multiply(4, 5)", multiply(4, 5), 20) && ok;
    ok = expect_equal("multiply(-3, 6)", multiply(-3, 6), -18) && ok;
    ok = expect_equal("multiply(0, 99)", multiply(0, 99), 0) && ok;

    if (!ok)
    {
        std::cerr << "One or more tests failed.\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}