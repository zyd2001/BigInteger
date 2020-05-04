#include "BigInteger.hpp"
#include "gtest/gtest.h"

namespace zyd2001
{
#define mag(a) BigIntegerTest::getMag(a)
#define sign(a) BigIntegerTest::getSign(a)
#define zeroTest(i)               \
    ASSERT_EQ(mag(i)->size(), 0); \
    ASSERT_EQ(sign(i), 0);
    class BigIntegerTest : public ::testing::Test
    {
    public:
        static BigInteger::VecPtr getMag(const BigInteger & n) { return n.magnitude; }
        static int getSign(const BigInteger & n) { return n.sign; }

        const static BigInteger zero;
        const static BigInteger::ElemType elemMax = BigInteger::elemMAX;
        const static BigInteger::SignedElemType signedElemMax = BigInteger::signedElemMax;
        const static BigInteger::SignedElemType signedElemMin = BigInteger::signedElemMin;
    };
    const BigInteger BigIntegerTest::zero = BigInteger::zero;
} // namespace zyd2001
