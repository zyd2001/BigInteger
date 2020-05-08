#include "BigInteger/BigIntegerTest.hpp"

using namespace zyd2001;
using namespace zyd2001::BigIntegerLiteral;
using DivTest = BigIntegerTest;

#define eq                                               \
    ASSERT_EQ(a / b, quo);                               \
    ASSERT_EQ(a / -b, -quo);                             \
    ASSERT_EQ(-a / b, -quo);                             \
    ASSERT_EQ(-a / -b, quo);                             \
    ASSERT_EQ(a % b, rem);                               \
    ASSERT_EQ(a % -b, rem);                              \
    ASSERT_EQ(-a % b, -rem);                             \
    ASSERT_EQ(-a % -b, -rem);                            \
    ASSERT_EQ(a.div(b), std::make_tuple(quo, rem));      \
    ASSERT_EQ(a.div(-b), std::make_tuple(-quo, rem));    \
    ASSERT_EQ((-a).div(b), std::make_tuple(-quo, -rem)); \
    ASSERT_EQ((-a).div(-b), std::make_tuple(quo, -rem));

TEST_F(DivTest, test0)
{
    BigInteger a, b, quo, rem;
    a = 0;
    b = 0;
    quo = 0;
    rem = 0;
    ASSERT_THROW(a / b, BigIntegerException);
    ASSERT_THROW(a % b, BigIntegerException);
    ASSERT_THROW(a.div(b), BigIntegerException);
}

TEST_F(DivTest, test1)
{
    BigInteger a, b, quo, rem;
    a = 0;
    b = 1;
    quo = 0;
    rem = 0;
    eq;
    b = "9999999999999999999999999999999999999999999999999999999999999999999999"_BI;
    eq;
}

TEST_F(DivTest, test2)
{
    BigInteger a, b, quo, rem;
    a = 5234;
    b = BigIntegerTest::signedElemMax;
    quo = 0;
    rem = 5234;
    eq;
    a = "52349999999999999999999999999999999"_BI;
    b = "9999999999999999999999999999999999999999999999999999999999999999999999"_BI;
    quo = 0;
    rem = "52349999999999999999999999999999999"_BI;
    eq;
    a = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    b = "9999999999999999999999999999999999999999999999999999999999999999999999"_BI;
    quo = 0;
    rem = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    eq;
}

TEST_F(DivTest, test3)
{
    BigInteger a, b, quo, rem;
    a = BigIntegerTest::signedElemMax;
    b = BigIntegerTest::signedElemMax;
    quo = 1;
    rem = 0;
    eq;
    a = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    b = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    quo = 1;
    rem = 0;
    eq;
}

TEST_F(DivTest, test4)
{
    BigInteger a, b, quo, rem;
    a = 2352335235;
    b = 1;
    quo = 2352335235;
    rem = 0;
    eq;
    a = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    b = 1;
    quo = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    rem = 0;
    eq;
}

TEST_F(DivTest, test5)
{
    BigInteger a, b, quo, rem;
    a = 2352335235;
    b = 42342;
    quo = 2352335235 / 42342;
    rem = 2352335235 % 42342;
    eq;
    a = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    b = 236346345;
    quo = "42310787585904914247774806925827433464223870269709480804537087"_BI;
    rem = 70602983;
    eq;
    a = "9999999999999999999999999999999999999999999999999999999999999999999998"_BI;
    b = "23609808996785676567456236346345"_BI;
    quo = "423552770010186681034249897120266176660"_BI;
    rem = "2204771965713296286759284692298"_BI;
    eq;
    a = "999999999999999999999999999999999999999999999999999999999999999909999999999999999999999999999999999999999"
        "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999345999998"_BI;
    b = "23609808996785676567455555555555555555555555555344444444444444444476236346345"_BI;
    quo = "4235527700101866810342621102993184033558617448606098658635383850955094861446529022014831334885257274619"
          "0118290596946601772867"_BI;
    rem = "3043552553670623098325109226340899869304870975097919173893257678119710378883"_BI;
    eq;
}