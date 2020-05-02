#include "BigIntegerTest.hpp"

using namespace zyd2001;
using ConstructorTest = BigIntegerTest;
TEST_F(ConstructorTest, defaultConstructor)
{
    BigInteger i;
    ASSERT_EQ(mag(i)->size(), 0);
    ASSERT_EQ(sign(i), 0);
}

TEST_F(ConstructorTest, constructFrom0)
{
    BigInteger i(0);
    ASSERT_EQ(mag(i)->size(), 0);
    ASSERT_EQ(sign(i), 0);
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
    ASSERT_EQ(mag(i)->at(0), -1);
    ASSERT_EQ(sign(i), -1);
}

TEST_F(ConstructorTest, constructFromINT_MAX)
{
    BigInteger i(INT_MAX);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), INT_MAX);
    ASSERT_EQ(sign(i), 1);
}

TEST_F(ConstructorTest, constructFromINT_MIN)
{
    BigInteger i(INT_MIN);
    ASSERT_EQ(mag(i)->size(), 1);
    ASSERT_EQ(mag(i)->at(0), INT_MAX + 1);
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
    ASSERT_EQ(mag(i)->size(), 0);
    ASSERT_EQ(sign(i), 0);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFromSpaceString)
{
    const char * str = "    \n \t";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 0);
    ASSERT_EQ(sign(i), 0);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}

TEST_F(ConstructorTest, constructFrom0String)
{
    const char * str = "   0000";
    BigInteger i(str);
    std::string ss(str);
    BigInteger s(ss);
    ASSERT_EQ(mag(i)->size(), 0);
    ASSERT_EQ(sign(i), 0);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
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
    const char * str = "-00abcdqwexzhahahaazahzrsdyidazazzxd";
    std::string ss(str);
    BigInteger i(str, 36);
    BigInteger s(ss, 36);
    ASSERT_EQ(mag(i)->size(), 3);
    ASSERT_EQ(mag(i)->at(0), 2776039359460685473UL);
    ASSERT_EQ(mag(i)->at(1), 12843823637769055684UL);
    ASSERT_EQ(mag(i)->at(2), 1920058322246UL);
    ASSERT_EQ(sign(i), -1);
    ASSERT_EQ(*mag(i), *mag(s));
    ASSERT_EQ(sign(i), sign(s));
}