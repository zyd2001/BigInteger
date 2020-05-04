#include "BigIntegerTest.hpp"

using namespace zyd2001;
using MulTest = BigIntegerTest;

#define eq                   \
    ASSERT_EQ(a * b, res);   \
    ASSERT_EQ(b * a, res);   \
    ASSERT_EQ(a * -b, -res); \
    ASSERT_EQ(b * -a, -res); \
    ASSERT_EQ(-a * -b, res); \
    ASSERT_EQ(-b * -a, res);

TEST_F(MulTest, test0)
{
    BigInteger a, b, res;
    a = 0;
    b = 12312312;
    res = 0;
    eq;
    a = "12343534637351443425645745734523412312577568563454234";
    b = 0;
    res = 0;
    eq;
}

TEST_F(MulTest, test1)
{
    BigInteger a, b, res;
    a = 1;
    b = 58867364;
    res = 58867364;
    eq;
    a = "12343534637351443425645745734523412312577568563454234";
    b = 1;
    res = "12343534637351443425645745734523412312577568563454234";
    eq;
    a = 12345;
    b = "12343534637351443425645745734523412312577568563454234";
    res = "152380935098103569089596731092691524998770083915842518730";
    eq;
}

TEST_F(MulTest, test2)
{
    BigInteger a, b, res;
    a = "18446744073709551615";
    b = "18446744073709551615";
    res = "340282366920938463426481119284349108225";
    eq;
    a = "12343534637351443425645745734523412312577568563454234";
    b = "12343534637351443425645745734523412312577568563454234";
    res = "1523628473434948299639319056129991099461240617097733833270455269922668581580812960937486035264978125267"
          "56";
    eq;
}