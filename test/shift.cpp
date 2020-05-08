#include "BigInteger/BigIntegerTest.hpp"

using namespace zyd2001;
using namespace zyd2001::BigIntegerLiteral;
using ShiftTest = BigIntegerTest;

#define eq ASSERT_EQ(i, res)

TEST_F(ShiftTest, test0)
{
    BigInteger i(0), res;
    i = i << 123;
    eq;
    i = 0;
    i >> 123;
    zeroTest(i);
    eq;
}

TEST_F(ShiftTest, test1)
{
    BigInteger i(1), res("10633823966279326983230456482242756608");
    i = i << 123;
    eq;
    res = BigInteger("576460752303423488");
    i = i >> 64;
    eq;
    res = 0;
    i = i >> 5000;
    zeroTest(i);
    eq;
}

TEST_F(ShiftTest, test2)
{
    BigInteger i, res;
    res = "-476902596631542253150345593789883897174029520308153588717524267397688251487844927853161637236578715866"
          "6617641286464646303430865140794911174853177787729764185248568392004348719604691684277261706844620439139"
          "6263494306740250034200508171185391779463962312157411608594387095802472530248591280503036615348719530820"
          "45135389962360584303392339923060825288296703165586996033454862975893504"_BI;
    i = -123;
    i = i << 1251;
    eq;
    res = -123;
    i = i >> 1251;
    ASSERT_EQ(mag(i)->size(), 1);
    eq;
    res = -61;
    i = i >> 1;
    eq;
    res = 0;
    i = i >> 6;
    zeroTest(i);
    eq;
}