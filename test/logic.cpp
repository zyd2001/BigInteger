#include "BigIntegerTest.hpp"

using namespace zyd2001;
using LogicTest = BigIntegerTest;

#define boole       \
    ASSERT_TRUE(a); \
    ASSERT_FALSE(!a);
#define note            \
    ASSERT_EQ(~a, res); \
    ASSERT_EQ(a, ~res);
#define ande               \
    ASSERT_EQ(a & b, res); \
    ASSERT_EQ(b & a, res); \
    ASSERT_EQ(a & a, a);   \
    ASSERT_EQ(b & b, b);
#define ore                \
    ASSERT_EQ(a | b, res); \
    ASSERT_EQ(b | a, res); \
    ASSERT_EQ(a | a, a);   \
    ASSERT_EQ(b | b, b);
#define xore               \
    ASSERT_EQ(a ^ b, res); \
    ASSERT_EQ(b ^ a, res); \
    zeroTest(a ^ a);       \
    zeroTest(b ^ b);

TEST_F(LogicTest, boolTest)
{
    BigInteger a;
    a = 1;
    boole;
    a = "-134124124124124155555555555555555555555524121";
    boole;
    a = 0;
    ASSERT_FALSE(a);
    ASSERT_TRUE(!a);
}

TEST_F(LogicTest, notTest)
{
    BigInteger a, res;
    a = 123;
    res = ~123;
    note;
    a = -123;
    res = ~-123;
    note;
    a = "1245234634674573406280693486036345932471";
    res = "-1245234634674573406280693486036345932472";
    note;
}

TEST_F(LogicTest, andTest0)
{
    BigInteger a, b, res;
    a = 123;
    b = 0;
    res = 0;
    zeroTest(a & b);
    zeroTest(b & a);
    ande;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = 0;
    res = 0;
    zeroTest(a & b);
    zeroTest(b & a);
    ande;
}

TEST_F(LogicTest, andTest1)
{
    BigInteger a, b, res;
    a = 123;
    b = 321;
    res = 65;
    ande;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = "55555555555555534634634634634";
    res = "9906429759049646979688850570";
    ande;
}

TEST_F(LogicTest, andTest2)
{
    BigInteger a, b, res;
    a = -123;
    b = 324125321;
    res = 324125313;
    ande;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = "-55555555555555534634634634634";
    res = "1245555555555555555555555346336439916587296699366657495554";
    ande;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = "55555555555555534634634634634";
    res = "45649125796505887654945784064";
    ande;
}

TEST_F(LogicTest, andTest3)
{
    BigInteger a, b, res;
    a = -123;
    b = -324125321;
    res = -324125435;
    ande;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = "-55555555555555534634634634634";
    res = "-1245555555555555555555555346391995472142852234001292130188";
    ande;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = -1;
    res = "-1245555555555555555555555346346346346346346346346346346123";
    ande;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = -1;
    res = "1245555555555555555555555346346346346346346346346346346123";
    ande;
}

TEST_F(LogicTest, orTest0)
{
    BigInteger a, b, res;
    a = 123;
    b = 0;
    res = 123;
    ore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = 0;
    res = "-1245555555555555555555555346346346346346346346346346346123";
    ore;
}

TEST_F(LogicTest, orTest1)
{
    BigInteger a, b, res;
    a = 123;
    b = 321;
    res = 379;
    ore;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = "55555555555555534634634634634";
    res = "1245555555555555555555555346391995472142852234001292130187";
    ore;
}

TEST_F(LogicTest, orTest2)
{
    BigInteger a, b, res;
    a = -123;
    b = 324125321;
    res = -123 | 324125321;
    ore;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = "-55555555555555534634634634634";
    res = "-45649125796505887654945784065";
    ore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = "55555555555555534634634634634";
    res = "-1245555555555555555555555346336439916587296699366657495553";
    ore;
}

TEST_F(LogicTest, orTest3)
{
    BigInteger a, b, res;
    a = -123;
    b = -324125321;
    res = -123 | -324125321;
    ore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = "-55555555555555534634634634634";
    res = "-9906429759049646979688850569";
    ore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = -1;
    res = -1;
    ore;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = -1;
    res = -1;
    ore;
}

TEST_F(LogicTest, xorTest0)
{
    BigInteger a, b, res;
    a = 123;
    b = 0;
    res = 123;
    xore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = 0;
    res = "-1245555555555555555555555346346346346346346346346346346123";
    xore;
}

TEST_F(LogicTest, xorTest1)
{
    BigInteger a, b, res;
    a = 123;
    b = 321;
    res = 123 ^ 321;
    xore;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = "55555555555555534634634634634";
    res = "1245555555555555555555555346382089042383802587021603279617";
    xore;
}

TEST_F(LogicTest, xorTest2)
{
    BigInteger a, b, res;
    a = -123;
    b = 324125321;
    res = -123 ^ 324125321;
    xore;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = "-55555555555555534634634634634";
    res = "-1245555555555555555555555346382089042383802587021603279619";
    xore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = "55555555555555534634634634634";
    res = "-1245555555555555555555555346382089042383802587021603279617";
    xore;
}

TEST_F(LogicTest, xorTest3)
{
    BigInteger a, b, res;
    a = -123;
    b = -324125321;
    res = -123 ^ -324125321;
    xore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = "-55555555555555534634634634634";
    res = "1245555555555555555555555346382089042383802587021603279619";
    xore;
    a = "-1245555555555555555555555346346346346346346346346346346123";
    b = -1;
    res = "1245555555555555555555555346346346346346346346346346346122";
    xore;
    a = "1245555555555555555555555346346346346346346346346346346123";
    b = -1;
    res = "-1245555555555555555555555346346346346346346346346346346124";
    xore;
}