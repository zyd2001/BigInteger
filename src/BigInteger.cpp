#include "BigInteger.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace zyd2001;

/**
 * @brief Construct a new BigInteger object from an integer
 *
 * @param i
 */
BigInteger::BigInteger(const SignedElemType i) : magnitude(zeroPtr)
{
    if (i == 0)
        sign = 0;
    else
    {
        ElemType n = i;
        this->magnitude = std::make_shared<Vec>(1);
        if (i < 0)
        {
            sign = -1;
            n = -i;
        }
        else
            sign = 1;
        (*magnitude)[0] = n;
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
BigInteger::BigInteger(const std::string & str, int base) : BigInteger(str.c_str(), base) {}

/**
 * @brief Construct a new BigInteger object from a base 10 char string
 *
 * @param str
 */
BigInteger::BigInteger(const char * str) : BigInteger(str, 10) {}

BigInteger::BigInteger(const char * str, int base) : magnitude(zeroPtr)
{
    if (base < 2 || base > 36)
        throw BigIntegerException("Invalid base");
    if (std::strlen(str) == 0)
        sign = 0;
    else
    {
        this->magnitude = std::make_shared<Vec>();
        sign = convert(*this->magnitude, str, base);
    }
}

BigInteger::BigInteger(const Vec e, int sign) : magnitude(std::make_shared<Vec>(e)), sign(sign) {}
BigInteger::BigInteger(const VecPtr e, int sign) : magnitude(e), sign(sign) {}

BigInteger BigInteger::operator+(const BigInteger & n) const
{
    if (this->sign == 0)
        return n;
    if (n.sign == 0)
        return *this;
    if (this == &n)
        return n << 1;
    if (this->sign == n.sign)
        return BigInteger(add(*this->magnitude, *n.magnitude), this->sign);
    else
    {
        auto & v1 = *this->magnitude;
        auto & v2 = *n.magnitude;
        int cmp = compare(v1, v2);
        if (cmp == 0)
            return zero;
        else
        {
            // large's sign dominates
            if (cmp > 0)
                return BigInteger(sub(v1, v2), this->sign);
            else
                return BigInteger(sub(v2, v1), n.sign);
        }
    }
}

BigInteger BigInteger::operator-(const BigInteger & n) const
{
    if (this == &n)
        return zero;
    return *this + -n;
}

BigInteger BigInteger::operator*(const BigInteger & n) const
{
    if (this->sign == 0 || n.sign == 0)
        return zero;
    if (this->sign == n.sign)
        return BigInteger(mul(*this->magnitude, *n.magnitude), 1);
    else
        return BigInteger(mul(*this->magnitude, *n.magnitude), -1);
}

BigInteger BigInteger::operator/(const BigInteger & n) const
{
    if (this->sign == 0)
        return zero;
    if (n.sign == 0)
        throw new BigIntegerException("Divide by zero");
    int cmp = compare(*this->magnitude, *n.magnitude);
    if (cmp < 0)
        return zero;
    if (cmp == 0)
        return BigInteger((this->sign == n.sign) ? 1 : -1);
    if (this->sign == n.sign)
        return BigInteger(div(*this->magnitude, *n.magnitude), 1);
    else
        return BigInteger(div(*this->magnitude, *n.magnitude), -1);
}

BigInteger BigInteger::operator%(const BigInteger & n) const
{
    if (this->sign == 0)
        return zero;
    if (n.sign == 0)
        throw new BigIntegerException("Divide by zero");
    int cmp = compare(*this->magnitude, *n.magnitude);
    if (cmp < 0)
        return *this;
    if (cmp == 0)
        return zero;
    // remainder's sign always follow dividend
    return BigInteger(std::get<1>(divrem(*this->magnitude, *n.magnitude)), this->sign);
}

BigInteger::QuoRemType BigInteger::div(const BigInteger & n) const
{
    if (this->sign == 0)
        return {zero, zero};
    if (n.sign == 0)
        throw new BigIntegerException("Divide by zero");
    int cmp = compare(*this->magnitude, *n.magnitude);
    if (cmp < 0)
        return {zero, *this};
    if (cmp == 0)
        return {BigInteger((this->sign == n.sign) ? 1 : -1), zero};

    // handle different sign
    int quoSign = (this->sign == n.sign) ? 1 : -1;
    VecPtr quo, rem;
    std::tie(quo, rem) = divrem(*this->magnitude, *n.magnitude);
    return {BigInteger(quo, quoSign), BigInteger(rem, this->sign)};
}

BigInteger BigInteger::operator-() const { return BigInteger(this->magnitude, -this->sign); }

BigInteger::operator bool() const { return this->sign != 0; }

BigInteger BigInteger::operator<<(const ElemType i) const
{
    if (i == 0)
        return *this;
    return BigInteger(sl(*this->magnitude, i), this->sign);
}
BigInteger BigInteger::operator>>(const ElemType i) const
{
    if (i == 0)
        return *this;
    if ((i / elemWIDTH) >= this->magnitude->size())
        return zero;
    return BigInteger(sr(*this->magnitude, i), this->sign);
}
BigInteger BigInteger::operator~() const
{
    if (sign == 0)
        return zero;
    else if (sign > 0)
        return BigInteger(add(*this->magnitude, 1), -1);
    else
        return BigInteger(sub(*this->magnitude, 1), 1);
}
BigInteger BigInteger::operator&(const BigInteger & n) const
{
    if (this == &n)
        return *this;
    if (this->sign == 0 || n.sign == 0)
        return zero;
    if (this->sign > 0 && n.sign > 0)
        return BigInteger(andFunc(*this->magnitude, *n.magnitude), 1);
    if (this->sign < 0 && n.sign < 0)
    {
        /* Both operands are negative, so will be the result.
            -((-OP1) & (-OP2)) = -(~(OP1 - 1) & ~(OP2 - 1)) =
            = ~(~(OP1 - 1) & ~(OP2 - 1)) + 1 =
            = ((OP1 - 1) | (OP2 - 1)) + 1      */
        auto && v1 = sub(*this->magnitude, 1);
        auto && v2 = sub(*n.magnitude, 1);
        return BigInteger(add(*orFunc(*v1, *v2), 1), -1);
    }
    if (this->sign < 0)
    {
        /* OP1 is positive and zero-extended,
            OP2 is negative and ones-extended.
            The result will be positive.
            OP1 & -OP2 = OP1 & ~(OP2 - 1).  */
        auto && v = sub(*this->magnitude, 1);
        return BigInteger(andnot(*n.magnitude, *v), 1);
    }
    {
        auto && v = sub(*n.magnitude, 1);
        return BigInteger(andnot(*this->magnitude, *v), 1);
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
    if (this->sign > 0 && n.sign > 0)
        return BigInteger(orFunc(*this->magnitude, *n.magnitude), 1);
    if (this->sign < 0 && n.sign < 0)
    {
        auto && v1 = sub(*this->magnitude, 1);
        auto && v2 = sub(*n.magnitude, 1);
        return BigInteger(add(*andFunc(*v1, *v2), 1), -1);
    }
    if (this->sign < 0)
    {
        /* Operand 2 negative, so will be the result.
        -(OP1 | (-OP2)) = -(OP1 | ~(OP2 - 1)) =
        = ~(OP1 | ~(OP2 - 1)) + 1 =
        = (~OP1 & (OP2 - 1)) + 1      */
        auto && v = sub(*this->magnitude, 1);
        return BigInteger(add(*andnot(*v, *n.magnitude), 1), -1);
    }
    {
        auto && v = sub(*n.magnitude, 1);
        return BigInteger(add(*andnot(*v, *this->magnitude), 1), -1);
    }
}
BigInteger BigInteger::operator^(const BigInteger & n) const
{
    if (this == &n)
        return zero;
    if (this->sign == 0)
        return n;
    if (n.sign == 0)
        return *this;
    if (this->sign > 0 && n.sign > 0)
        return BigInteger(xorFunc(*this->magnitude, *n.magnitude), 1);
    if (this->sign < 0 && n.sign < 0)
    {
        /* Both operands are negative, the result will be positive.
            (-OP1) ^ (-OP2) =
            = ~(OP1 - 1) ^ ~(OP2 - 1) =
            = (OP1 - 1) ^ (OP2 - 1)  */
        auto && v1 = sub(*this->magnitude, 1);
        auto && v2 = sub(*n.magnitude, 1);
        return BigInteger(xorFunc(*v1, *v2), 1);
    }
    if (this->sign < 0)
    {
        /* Operand 2 negative, so will be the result.
        -(OP1 ^ (-OP2)) = -(OP1 ^ ~(OP2 - 1)) =
        = ~(OP1 ^ ~(OP2 - 1)) + 1 =
        = (OP1 ^ (OP2 - 1)) + 1      */
        auto && v = sub(*this->magnitude, 1);
        return BigInteger(add(*xorFunc(*v, *n.magnitude), 1), -1);
    }
    {
        auto && v = sub(*n.magnitude, 1);
        return BigInteger(add(*xorFunc(*v, *this->magnitude), 1), -1);
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
        else
            if (this->sign == 1)
                return compare(*this->magnitude, *n.magnitude) > 0;
            else
                return compare(*this->magnitude, *n.magnitude) < 0;
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
        else
            if (this->sign == 1)
                return compare(*this->magnitude, *n.magnitude) < 0;
            else
                return compare(*this->magnitude, *n.magnitude) > 0;
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
            return (compare(*this->magnitude, *n.magnitude) == 0);
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

std::string BigInteger::toString() const { return toString(10); }
std::string BigInteger::toString(int base) const { return toString(base, false); }