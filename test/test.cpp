#include "gtest/gtest.h"

TEST(test, test1add1)
{
    ASSERT_EQ(1 + 1, 3) << "error";
}