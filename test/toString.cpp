#include "BigIntegerTest.hpp"

using namespace zyd2001;
using toStringTest = BigIntegerTest;
TEST_F(toStringTest, test0)
{
    BigInteger i;
    ASSERT_EQ(i.toString(), "0");
}

TEST_F(toStringTest, test1)
{
    BigInteger i(100000000000000000L);
    ASSERT_EQ(i.toString(16), "16345785d8a0000");
}

TEST_F(toStringTest, test2)
{
    BigInteger i(100000000000000000L);
    ASSERT_EQ(i.toString(16, true), "16345785D8A0000");
}

TEST_F(toStringTest, test3)
{
    BigInteger i("abchxjfgdksjdhfqiuewyeopidufasasfrgsfewvdxcvzdf", 36);
    ASSERT_EQ(i.toString(36), "abchxjfgdksjdhfqiuewyeopidufasasfrgsfewvdxcvzdf");
}

TEST_F(toStringTest, test4)
{
    BigInteger i("-abchxjfgdksjdhfqiuewyeopidufasasfrgsfewvdxcvzdf", 36);
    ASSERT_EQ(i.toString(36, true), "-ABCHXJFGDKSJDHFQIUEWYEOPIDUFASASFRGSFEWVDXCVZDF");
}

TEST_F(toStringTest, test5)
{
    BigInteger i;
    ASSERT_THROW(i.toString(1), BigIntegerException);
    ASSERT_THROW(i.toString(37), BigIntegerException);
}