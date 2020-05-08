#include "BigInteger/BigIntegerTest.hpp"

using namespace zyd2001;
using compareTest = BigIntegerTest;

#define gt(i, j)              \
    EXPECT_EQ(i == j, false); \
    EXPECT_EQ(i != j, true);  \
    EXPECT_EQ(i >= j, true);  \
    EXPECT_EQ(i <= j, false); \
    EXPECT_EQ(i > j, true);   \
    EXPECT_EQ(i < j, false);

#define lt(i, j)              \
    EXPECT_EQ(i == j, false); \
    EXPECT_EQ(i != j, true);  \
    EXPECT_EQ(i >= j, false); \
    EXPECT_EQ(i <= j, true);  \
    EXPECT_EQ(i > j, false);  \
    EXPECT_EQ(i < j, true);

#define eq(i, j)              \
    EXPECT_EQ(i == j, true);  \
    EXPECT_EQ(i != j, false); \
    EXPECT_EQ(i >= j, true);  \
    EXPECT_EQ(i <= j, true);  \
    EXPECT_EQ(i > j, false);  \
    EXPECT_EQ(i < j, false);

TEST_F(compareTest, test0)
{
    BigInteger i, j;
    eq(i, j);
}

TEST_F(compareTest, test1)
{
    BigInteger i(123), j(0);
    gt(i, j);
}

TEST_F(compareTest, test2)
{
    BigInteger i(-123), j(0);
    lt(i, j);
}

TEST_F(compareTest, test3)
{
    BigInteger i(-123), j(-123);
    eq(i, j);
}

TEST_F(compareTest, test4)
{
    BigInteger i(-123), j(-1);
    lt(i, j);
}

TEST_F(compareTest, test5)
{
    BigInteger i(123), j(1);
    gt(i, j);
}

TEST_F(compareTest, test6)
{
    BigInteger i("1231233"), j("123123123123123123123123123123134");
    lt(i, j);
}

TEST_F(compareTest, test7)
{
    BigInteger i("123123123123123123123123123123133"), j("123123123123123123123123123123133");
    eq(i, j);
}

TEST_F(compareTest, test8)
{
    BigInteger i("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz", 36), j("1231231231231");
    gt(i, j);
}

TEST_F(compareTest, test9)
{
    BigInteger i("-98798798798798798731231534636346155"), j("-98798798798798798731231534636346155");
    eq(i, j);
}

TEST_F(compareTest, test10)
{
    BigInteger i("-98798798798798798731231534636346156"), j(-98798798798798);
    lt(i, j);
}

TEST_F(compareTest, test11)
{
    BigInteger i(-98798798), j("-98798798798798798731231534636346155");
    gt(i, j);
}

TEST_F(compareTest, test12)
{
    BigInteger i(98798798), j("-98798798798798798731231534636346155");
    gt(i, j);
}

TEST_F(compareTest, test13)
{
    BigInteger i("-98798798798798798731231534636346156"), j(98798798798798);
    lt(i, j);
}