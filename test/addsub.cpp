#include "BigIntegerTest.hpp"

using namespace zyd2001;
using AddSubTest = BigIntegerTest;

#define eq                  \
    ASSERT_EQ(a + b, res);  \
    ASSERT_EQ(b + a, res);  \
    ASSERT_EQ(a - -b, res); \
    ASSERT_EQ(b - -a, res);

#define e ASSERT_EQ(-a, res)

TEST_F(AddSubTest, NegateTest0)
{
    BigInteger a, res;
    a = 1;
    res = -1;
    e;
    a = "12323232323232323232323232323234235253252523523523523";
    res = "-12323232323232323232323232323234235253252523523523523";
    e;
}

TEST_F(AddSubTest, AddSubTest0)
{
    BigInteger a, b, res;
    a = 1;
    b = 0;
    res = 1;
    eq;
    a = -1;
    b = 0;
    res = -1;
    eq;
    a = 0;
    b = "2344444444444444444444100000000000000000000000000555555555534";
    res = "2344444444444444444444100000000000000000000000000555555555534";
    eq;
    a = 0;
    b = "-1234455676781708570293802394803294";
    res = "-1234455676781708570293802394803294";
    eq;
}

TEST_F(AddSubTest, AddSubTest1)
{
    BigInteger a, b, res;
    a = -1;
    b = 1;
    res = 0;
    eq;
    a = BigInteger("-xzcjagdqhyvfuisdyggfiuh2349142813432tgrdsbvsdfsf", 36);
    b = BigInteger("xzcjagdqhyvfuisdyggfiuh2349142813432tgrdsbvsdfsf", 36);
    res = 0;
    eq;
}

TEST_F(AddSubTest, AddSubTest2)
{
    BigInteger a, b, res;
    a = -123;
    b = 1234;
    res = 1111;
    eq;
    a = BigInteger("abc", 16);
    b = 2345;
    res = BigInteger("13E5", 16);
    eq;
    a = "123123154230534759843759720496501983740958742059475209847193264895734";
    b = "5634673934763476349856239456193696740752653678365858673697428561986748924";
    res = "5634797057917706884616083215914193242736394637107918148907275755251644658";
    eq;
    // TODO: Type
    // carry
    a = "340282366920938463463374607431768211455"; // MAX,MAX
    b = "1";
    res = "340282366920938463463374607431768211456"; // 1,0,0
    eq;
    a = "340282366920938463463374607431768211456";
    b = "-1";
    res = "340282366920938463463374607431768211455";
    eq;

    a = -1234;
    b = -2345;
    res = -1234 - 2345;
    eq;
    a = "-9126236356463453453453453452412412421415235346356252353463454635231412423648987967886";
    b = "-2352346356463453123968707489127694683754630101410293589376376384231412423648987967886";
    res = "-11478582712926906577422160941540107105169865447766545942839831019462824847297975935772";
    eq;

    a = -1234;
    b = 2345;
    res = -1234 + 2345;
    eq;
    a = "-349867967249857934752374237402375736730572405730572905782342342342342342093570275239047";
    b = "128309128074012974012740912704712904701740127407757676745764372774237";
    res = "-349867967249857934624065109328362762717831493025860001080602214934584665347805902464810";
    eq;
}