#include <gtest/gtest.h>
#include "math_utils.h"

TEST(CoreMathTests, AddPositiveNumbers)
{
    EXPECT_EQ(add(2, 3), 5);
}

TEST(CoreMathTests, AddNegativeAndPositive)
{
    EXPECT_EQ(add(-4, 1), -3);
}

TEST(CoreMathTests, MultiplyPositiveNumbers)
{
    EXPECT_EQ(multiply(4, 5), 20);
}

TEST(CoreMathTests, MultiplyNegativeAndPositive)
{
    EXPECT_EQ(multiply(-3, 6), -18);
}

TEST(CoreMathTests, MultiplyByZero)
{
    EXPECT_EQ(multiply(0, 99), 0);
}