#include "BigIntegerTest.hpp"

using namespace zyd2001;
using namespace zyd2001::BigIntegerLiteral;
using ConstructorTest = BigIntegerTest;
TEST_F(ConstructorTest, defaultConstructor)
{
    BigInteger i;
    zeroTest(i);
}

TEST_F(ConstructorTest, constructFrom0)
{
    BigInteger i(0), j;
    zeroTest(i);
    ASSERT_EQ(mag(i), mag(j));
}

TEST_F(ConstructorTest, constructFrom1)
{
    BigInteger i(1);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1);
    ASSERT_EQ(sign(i), 1);
}

TEST_F(ConstructorTest, constructFromN1)
{
    BigInteger i(-1);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1);
    ASSERT_EQ(sign(i), -1);
}

TEST_F(ConstructorTest, constructFromINT_MAX)
{
    BigInteger i(std::numeric_limits<int>::max());
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), std::numeric_limits<int>::max());
    ASSERT_EQ(sign(i), 1);
}

TEST_F(ConstructorTest, constructFromINT_MIN)
{
    BigInteger i(std::numeric_limits<int>::min());
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), static_cast<BigInteger::SignedElemType>(std::numeric_limits<int>::max()) + 1);
    ASSERT_EQ(sign(i), -1);
}

TEST_F(ConstructorTest, constructFromSignElemMAX)
{
    BigInteger::SignedElemType max = BigIntegerTest::signedElemMax;
    BigInteger i(max);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), max);
    ASSERT_EQ(sign(i), 1);
}

TEST_F(ConstructorTest, constructFromSignElemMIN)
{
    BigInteger i(BigIntegerTest::signedElemMin);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), static_cast<BigInteger::ElemType>(BigIntegerTest::signedElemMax) + 1);
    ASSERT_EQ(sign(i), -1);
}

TEST_F(ConstructorTest, constructFromEmptyString)
{
    const char * str = "";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    zeroTest(i);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromSpaceString)
{
    const char * str = "    \n \t";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    zeroTest(i);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFrom0String)
{
    const char * str = "   0000";
    const char * str1 = "  -0000";
    BigInteger i(str), i1(str1);
    std::string ss(str), ss1(str1);
    BigInteger s(ss), s1(ss1);
    zeroTest(i);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
    zeroTest(i1);
    ASSERT_EQ(*mag(i1), *mag(s1));
    ASSERT_EQ(sign(i1), sign(s1));
}

TEST_F(ConstructorTest, constructFromBase10String1)
{
    const char * str = "1234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1234);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String2)
{
    const char * str = "-1234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1234);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String3)
{
    const char * str = "00001234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1234);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String4)
{
    const char * str = "-00001234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1234);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String5)
{
    const char * str = "00001234 123";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1234);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String6)
{
    const char * str = "-00001234 123";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 1234);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

// TODO: Type
TEST_F(ConstructorTest, constructFromBase10String7)
{
    const char * str = "12341234123412341234123412341234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 2);
    ASSERT_EQ(mag(i)->at(0), 14665443480974393842UL);
    ASSERT_EQ(mag(i)->at(1), 669019642387UL);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String8)
{
    const char * str = "-12341234123412341234123412341234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 2);
    ASSERT_EQ(mag(i)->at(0), 14665443480974393842UL);
    ASSERT_EQ(mag(i)->at(1), 669019642387UL);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromBase10String9)
{
    const char * str = "1234123412341234125453533453453453453453434123412341234";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 3);
    ASSERT_EQ(mag(i)->at(0), 10041519148592038386UL);
    ASSERT_EQ(mag(i)->at(1), 2386905687834773895UL);
    ASSERT_EQ(mag(i)->at(2), 3626762748561613UL);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

// TODO: Type
TEST_F(ConstructorTest, constructFromBase10String10)
{
    const char * str = "340282366920938463463374607431768211456";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 3);
    ASSERT_EQ(mag(i)->at(0), 0);
    ASSERT_EQ(mag(i)->at(1), 0);
    ASSERT_EQ(mag(i)->at(2), 1);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromStringException)
{
    const char * str1 = "0-0001234";
    const char * str2 = "a";
    const char * str3 = "  0a";
    const char * str4 = "  aabcz";
    const char * str5 = "  00123df zz";
    std::string ss1(str1);
    std::string ss2(str2);
    std::string ss3(str3);
    std::string ss4(str4);
    std::string ss5(str5);
    ASSERT_THROW(BigInteger i(str1);, BigIntegerException);
    ASSERT_THROW(BigInteger i(ss1);, BigIntegerException);
    ASSERT_THROW(BigInteger i(str2);, BigIntegerException);
    ASSERT_THROW(BigInteger i(ss2);, BigIntegerException);
    ASSERT_THROW(BigInteger i(str3);, BigIntegerException);
    ASSERT_THROW(BigInteger i(ss3);, BigIntegerException);
    ASSERT_THROW(BigInteger i(str4, 35);, BigIntegerException);
    ASSERT_THROW(BigInteger i(ss4, 35);, BigIntegerException);
    ASSERT_THROW(BigInteger i(ss4, 37);, BigIntegerException);
    ASSERT_THROW(BigInteger i(ss4, 1);, BigIntegerException);
    ASSERT_NO_THROW(BigInteger i(str5, 16););
    ASSERT_NO_THROW(BigInteger i(ss5, 16););
}

TEST_F(ConstructorTest, constructFromOtherBaseString1)
{
    const char * str = "00abc";
    std::string ss(str);
    BigInteger i(str, 18);
    BigInteger s(ss, 18);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 3450);
    ASSERT_EQ(sign(i), 1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromOtherBaseString2)
{
    const char * str = "-00abc";
    std::string ss(str);
    BigInteger i(str, 16);
    BigInteger s(ss, 16);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 0xabc);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromOtherBaseString3)
{
    const char * str = "-00ABC";
    std::string ss(str);
    BigInteger i(str, 16);
    BigInteger s(ss, 16);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), 0xabc);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromOtherBaseString4)
{
    const char * str = "-00abcdqwexzhahahaazahzrsdyidazazzxd1234567890";
    std::string ss(str);
    BigInteger i(str, 36);
    BigInteger s(ss, 36);
    ASSERT_EQ(mag(i)->size(), 4);
    ASSERT_EQ(mag(i)->at(0), 1112775687446536068UL);
    ASSERT_EQ(mag(i)->at(1), 6134917919516943653UL);
    ASSERT_EQ(mag(i)->at(2), 3410682659916918515UL);
    ASSERT_EQ(mag(i)->at(3), 380556992UL);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, copyConstructorTest0)
{
    BigInteger i, j(123123);
    BigInteger c(-1234567890);
    i = j;
    ASSERT_EQ(mag(i), mag(j));
    ASSERT_EQ(sign(i), sign(j));
    j = c;
    ASSERT_EQ(mag(c), mag(j));
    ASSERT_EQ(sign(c), sign(j));
}

TEST_F(ConstructorTest, stringLiteralTest)
{
    BigInteger i = "018275349876918276983479"_BI;
    BigInteger j("018275349876918276983479");
    ASSERT_EQ(*mag(i), *mag(j));
    ASSERT_EQ(sign(i), sign(j));
}