#include "BigInteger/BigInteger.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <istream>
#include <ostream>
#include <sstream>

using namespace zyd2001;

bool BigInteger::equalNegOne(const BigInteger & n)
{
    if (n.magnitude.size > 1)
        return false;
    if (n.sign != -1)
        return false;
    if (n.magnitude.back() == 1)
        return true;
    return false;
}

/**
 * @brief Construct a new BigInteger object from an integer
 *
 * @param i
 */
BigInteger::BigInteger(const SignedElemType i)
{
    if (i == 0)
        this->sign = 0;
    else
    {
        ElemType n = i;
        this->magnitude = Magnitude(1);
        if (i < 0)
        {
            this->sign = -1;
            n = -i;
        }
        else
            this->sign = 1;
        magnitude[0] = n;
    }
}

BigInteger::BigInteger(const int i) : BigInteger(static_cast<SignedElemType>(i)) {}

/**
 * @brief Construct a new BigInteger object from a base 10 string
 *
 * @param str
 */
BigInteger::BigInteger(const std::string & str) : BigInteger(str, 10) {}

/**
 * @brief Construct a new BigInteger object from a string with custom base
 *
 * @param str
 * @param base 2 < base <= 36
 */
BigInteger::BigInteger(const std::string & str, int base) : BigInteger(str.c_str(), str.length(), base) {}

BigInteger::BigInteger(const char * str, std::size_t len, int base)
{
    if (base < 2 || base > 36)
        throw BigIntegerException("Invalid base");
    if (std::strlen(str) == 0)
        this->sign = 0;
    else
        std::tie(this->magnitude, this->sign) = Magnitude::convert(str, len, base);
}

BigInteger BigIntegerLiteral::operator"" _BI(const char * s, std::size_t i) { return BigInteger(s, i, 10); }

BigInteger::BigInteger(Magnitude && m, int sign) : magnitude(std::move(m)), sign(sign) {}

BigInteger BigInteger::operator+(const BigInteger & n) const
{
    if (this->sign == 0)
        return n;
    if (n.sign == 0)
        return *this;
    if (this == &n)
        return n << 1;
    if (this->sign == n.sign)
        return BigInteger(this->magnitude + n.magnitude, this->sign);
    else
    {
        auto & v1 = this->magnitude;
        auto & v2 = n.magnitude;
        int cmp = v1.compare(v2);
        if (cmp == 0)
            return 0;
        else
        {
            // large's sign dominates
            if (cmp > 0)
                return BigInteger(v1 - v2, this->sign);
            else
                return BigInteger(v2 - v1, n.sign);
        }
    }
}

BigInteger BigInteger::operator-(const BigInteger & n) const
{
    if (this == &n)
        return 0;
    if (this->sign == 0)
        return -n;
    if (n.sign == 0)
        return *this;

    auto & v1 = this->magnitude;
    auto & v2 = n.magnitude;
    if (this->sign == n.sign)
    {
        auto & v1 = this->magnitude;
        auto & v2 = n.magnitude;
        int cmp = v1.compare(v2);
        if (cmp == 0)
            return 0;
        else
        {
            if (cmp > 0)
                return BigInteger(v1 - v2, this->sign);
            else
                return BigInteger(v2 - v1, -this->sign);
        }
    }
    else
        return BigInteger(this->magnitude + n.magnitude, this->sign);
}

BigInteger BigInteger::operator*(const BigInteger & n) const
{
    if (this->sign == 0 || n.sign == 0)
        return 0;
    if (this->sign == n.sign)
        return BigInteger(this->magnitude * n.magnitude, 1);
    else
        return BigInteger(this->magnitude * n.magnitude, -1);
}

BigInteger BigInteger::operator/(const BigInteger & n) const
{
    if (n.sign == 0)
        throw BigIntegerException("Divide by 0");
    if (this->sign == 0)
        return 0;
    int cmp = this->magnitude.compare(n.magnitude);
    if (cmp < 0)
        return 0;
    if (cmp == 0)
        return BigInteger((this->sign == n.sign) ? 1 : -1);
    if (this->sign == n.sign)
        return BigInteger(this->magnitude / n.magnitude, 1);
    else
        return BigInteger(this->magnitude / n.magnitude, -1);
}

BigInteger BigInteger::operator%(const BigInteger & n) const
{
    if (n.sign == 0)
        throw BigIntegerException("Divide by 0");
    if (this->sign == 0)
        return 0;
    int cmp = this->magnitude.compare(n.magnitude);
    if (cmp < 0)
        return *this;
    if (cmp == 0)
        return 0;
    // remainder's sign always follow dividend
    // handle remainder 0
    auto [quo, rem] = this->magnitude.divrem(n.magnitude);
    if (rem.size == 0)
        return 0;
    return BigInteger(std::move(rem), this->sign);
}

BigInteger::QuoRemType BigInteger::div(const BigInteger & n) const
{
    if (n.sign == 0)
        throw BigIntegerException("Divide by 0");
    if (this->sign == 0)
        return {0, 0};
    int cmp = this->magnitude.compare(n.magnitude);
    if (cmp < 0)
        return {0, *this};
    if (cmp == 0)
        return {BigInteger((this->sign == n.sign) ? 1 : -1), 0};

    // handle different sign
    int quoSign = (this->sign == n.sign) ? 1 : -1;
    auto [quo, rem] = this->magnitude.divrem(n.magnitude);
    if (rem.size == 0)
        return {BigInteger(std::move(quo), quoSign), 0};
    return {BigInteger(std::move(quo), quoSign), BigInteger(std::move(rem), this->sign)};
}

BigInteger BigInteger::operator-() const
{
    // will copy
    return BigInteger(Magnitude(this->magnitude), -this->sign);
}

BigInteger::operator bool() const { return this->sign != 0; }

BigInteger BigInteger::operator<<(const ElemType i) const
{
    if (i == 0)
        return *this;
    if (this->sign == 0)
        return 0;
    return BigInteger(this->magnitude << i, this->sign);
}
BigInteger BigInteger::operator>>(const ElemType i) const
{
    if (i == 0)
        return *this;
    if (this->sign == 0)
        return 0;
    if ((i / elemWIDTH) >= this->magnitude.size)
        return 0;
    // prevent become 0 but have sign
    auto && ptr = this->magnitude >> i;
    if (ptr.size == 0)
        return 0;
    else
        return BigInteger(std::move(ptr), this->sign);
}
BigInteger BigInteger::operator~() const
{
    if (sign == 0)
        return 0;
    else if (sign > 0)
        return BigInteger(this->magnitude + 1, -1);
    else
        return BigInteger(this->magnitude - 1, 1);
}
BigInteger BigInteger::operator&(const BigInteger & n) const
{
    if (this == &n)
        return *this;
    if (this->sign == 0 || n.sign == 0)
        return 0;
    if (equalNegOne(*this))
        return n;
    if (equalNegOne(n))
        return *this;
    if (this->sign > 0 && n.sign > 0)
        return BigInteger(this->magnitude & n.magnitude, 1);
    if (this->sign < 0 && n.sign < 0)
    {
        /* Both operands are negative, so will be the result.
            -((-OP1) & (-OP2)) = -(~(OP1 - 1) & ~(OP2 - 1)) =
            = ~(~(OP1 - 1) & ~(OP2 - 1)) + 1 =
            = ((OP1 - 1) | (OP2 - 1)) + 1      */
        auto && v1 = this->magnitude - 1;
        auto && v2 = n.magnitude - 1;
        return BigInteger((v1 | v2) + 1, -1);
    }
    if (this->sign < 0)
    {
        /* OP1 is positive and 0-extended,
            OP2 is negative and ones-extended.
            The result will be positive.
            OP1 & -OP2 = OP1 & ~(OP2 - 1).  */
        auto && v = this->magnitude - 1;
        return BigInteger(n.magnitude.andnot(v), 1);
    }
    // n.sign < 0
    {
        auto && v = n.magnitude - 1;
        return BigInteger(this->magnitude.andnot(v), 1);
    }
}
BigInteger BigInteger::operator|(const BigInteger & n) const
{
    if (this == &n)
        return *this;
    if (this->sign == 0)
        return n;
    if (n.sign == 0)
        return *this;
    if (equalNegOne(*this) || equalNegOne(n))
        return -1;
    if (this->sign > 0 && n.sign > 0)
        return BigInteger(this->magnitude | n.magnitude, 1);
    if (this->sign < 0 && n.sign < 0)
    {
        auto && v1 = this->magnitude - 1;
        auto && v2 = n.magnitude - 1;
        // andFunc can result 0, but we eliminate this case
        // when v1 == 0 || v2 == 0 || v1 == -1 || v2 == -1
        auto && a = v1 & v2;
        assert(a.size != 0);
        return BigInteger(a + 1, -1);
    }
    if (this->sign < 0)
    {
        /* Operand 2 negative, so will be the result.
        -(OP1 | (-OP2)) = -(OP1 | ~(OP2 - 1)) =
        = ~(OP1 | ~(OP2 - 1)) + 1 =
        = (~OP1 & (OP2 - 1)) + 1      */
        auto && v = this->magnitude - 1;
        // andnot won't be 0 unless all are 0, won't happen
        return BigInteger(v.andnot(n.magnitude) + 1, -1);
    }
    // n.sign < 0
    {
        auto && v = n.magnitude - 1;
        return BigInteger(v.andnot(this->magnitude) + 1, -1);
    }
}
BigInteger BigInteger::operator^(const BigInteger & n) const
{
    if (this == &n)
        return 0;
    if (this->sign == 0)
        return n;
    if (n.sign == 0)
        return *this;
    if (equalNegOne(*this))
        return ~n;
    if (equalNegOne(n))
        return ~(*this);
    if (this->sign > 0 && n.sign > 0)
    {
        // when equal, result will be 0
        // not worth compare
        auto && x = this->magnitude ^ n.magnitude;
        if (x.size == 0)
            return 0;
        return BigInteger(std::move(x), 1);
    }
    if (this->sign < 0 && n.sign < 0)
    {
        /* Both operands are negative, the result will be positive.
            (-OP1) ^ (-OP2) =
            = ~(OP1 - 1) ^ ~(OP2 - 1) =
            = (OP1 - 1) ^ (OP2 - 1)  */
        auto && v1 = this->magnitude - 1;
        auto && v2 = n.magnitude - 1;
        auto && x = v1 ^ v2;
        if (x.size == 0)
            return 0;
        return BigInteger(std::move(x), 1);
    }
    if (this->sign < 0)
    {
        /* Operand 2 negative, so will be the result.
        -(OP1 ^ (-OP2)) = -(OP1 ^ ~(OP2 - 1)) =
        = ~(OP1 ^ ~(OP2 - 1)) + 1 =
        = (OP1 ^ (OP2 - 1)) + 1      */
        auto && v = this->magnitude - 1;
        return BigInteger((v ^ n.magnitude) + 1, -1);
    }
    // n.sign < 0
    {
        auto && v = n.magnitude - 1;
        return BigInteger((v ^ this->magnitude) + 1, -1);
    }
}

bool BigInteger::operator>(const BigInteger & n) const
{
    if (this == &n)
        return false;
    if (this->sign > n.sign)
        return true;
    if (this->sign == n.sign)
        if (this->sign == 0)
            return false;
        else if (this->sign == 1)
            return this->magnitude.compare(n.magnitude) > 0;
        else
            return this->magnitude.compare(n.magnitude) < 0;
    else
        return false;
}
bool BigInteger::operator>=(const BigInteger & n) const { return !(*this < n); }
bool BigInteger::operator<(const BigInteger & n) const
{
    if (this == &n)
        return false;
    if (this->sign < n.sign)
        return true;
    if (this->sign == n.sign)
        if (this->sign == 0)
            return false;
        else if (this->sign == 1)
            return this->magnitude.compare(n.magnitude) < 0;
        else
            return this->magnitude.compare(n.magnitude) > 0;
    else
        return false;
}
bool BigInteger::operator<=(const BigInteger & n) const { return !(*this > n); }
bool BigInteger::operator==(const BigInteger & n) const
{
    if (this == &n)
        return true;
    if (this->sign == n.sign)
        if (this->sign == 0)
            return true;
        else
            return (this->magnitude.compare(n.magnitude) == 0);
    else
        return false;
}
bool BigInteger::operator!=(const BigInteger & n) const { return !(*this == n); }

std::ostream & zyd2001::operator<<(std::ostream & o, const BigInteger & i)
{
    o << i.toString();
    return o;
}
std::istream & zyd2001::operator>>(std::istream & i, BigInteger & num)
{
    std::string str;
    i >> str;
    num = BigInteger(str);
    return i;
}

void BigInteger::negate() { sign = -sign; }

std::string BigInteger::toString() const { return toString(10); }
std::string BigInteger::toString(int base) const { return toString(base, false); }
std::string BigInteger::toString(int base, bool upper) const
{
    auto && str = this->magnitude.toRawString(base, upper);
    if (this->sign < 0)
        str.push_back('-');
    std::reverse(str.begin(), str.end());
    return str;
}