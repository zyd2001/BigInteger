#include "BigInteger.hpp"
#include <cctype>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace zyd2001;
using namespace std;

const int BigInteger::digitsPerInt[35] = {31, 19, 15, 13, 11,
        11, 10, 9, 9, 8, 8, 8, 8, 7, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5};
const BigInteger BigInteger::zero = BigInteger();

/**
 * @brief helper Function to convert char to int
 * 
 * @param ch 
 * @param base 
 * @return int -1 when ch is invalid
 */
int BigInteger::digit(char ch, int base)
{
    if (base > 10)
    {
        if (ch >= '0' && ch < '9')
            return ch - '0';
        else if (ch >= 'a' && ch < 'a' + base - 10)
            return ch - 'a' + 10;
        else
            return -1;
    }
    else
    {
        if (ch >= '0' && ch < '0' + base)
        {
            return ch - '0';
        }
        else
            return -1;
    }
}

int BigInteger::compare(const magVec & v1, const magVec & v2)
{
    if (v1.size() > v2.size())
        return 1;
    else if (v1.size() < v2.size())
        return -1;
    else
    {
        for (std::size_t i = v1.size() - 1; i >= 0; --i)
        {
            if (v1[i] != v2[i])
                return (v1[i] > v2[i]) ? 1 : -1;
        }
        return 0;
    }
}

// swap two arguments to make sure v1 is large one
#define swap_and_define \
    auto & v1 = (a.size() < b.size()) ? b : a; \
    auto & v2 = (a.size() < b.size()) ? a : b; \
    auto maxSize = v1.size(); \
    auto minSize = v2.size();

BigInteger::magType BigInteger::add(const magVec & a, const magVec & b)
{
    int carry = 0, ca1, ca2;
    swap_and_define
    auto res = std::make_shared<magVec>(maxSize + 1);
    auto & r = *res;
    elemType s1, s2;
    for (std::size_t i = 0; i < minSize; ++i)
    {
        s1 = v1[i] + v2[i];
        ca1 = s1 < v1[i];
        s2 = s1 + carry;
        ca2 = s2 == 0;
        carry = ca1 | ca2;
        r[i] = s2;
    }
    for (std::size_t i = minSize; i < maxSize && carry != 0; ++i)
    {
        s1 = v1[i] + carry;
        carry = s1 == 0; // if overflow, only possibility is 0
        r[i] = s1;
    }
    if (carry)
        r.back() = 1;
    else
        r.resize(maxSize);
    return res;
}

BigInteger::magType BigInteger::add(const magVec & v1, const elemType n)
{
    int carry = 0;
    auto res = std::make_shared<magVec>(v1.size() + 1);
    auto & r = *res;
    elemType sl;
    sl = v1[0] + n;
    carry = sl < v1[0];
    r[0] = sl;
    for (std::size_t i = 1; i < v1.size() && carry != 0; i++)
    {
        sl = v1[i] + carry;
        carry = sl == 0; // if overflow, only possibility is 0
        r[i] = sl;
    }
    if (carry)
        r.back() = 1;
    else 
        r.resize(v1.size());
    return res;
}

// assume v1 > v2
BigInteger::magType BigInteger::sub(const magVec & v1, const magVec & v2)
{
    int borrow = 0, br1, br2;
    auto res = std::make_shared<magVec>(v1.size());
    auto & r = *res;
    elemType s1, s2;
    for (std::size_t i = 0; i < v2.size(); ++i)
    {
        // if overflow, the result will just be uintmax + v1[i] - v[2]
        // because of 2's complement
        s1 = v1[i] - v2[i];
        br1 = s1 > v1[i];
        s2 = s1 - borrow;
        br2 = s2 == elemMAX;
        borrow = br1 | br2;
        r[i] = s2;
    }
    for (std::size_t i = v2.size(); i < v1.size() && borrow != 0; ++i)
    {
        s1 = v1[i] - borrow;
        borrow = s1 == elemMAX;
        r[i] = s1;
    }
    return res;
}

// assume v1 > n
BigInteger::magType BigInteger::sub(const magVec & v1, const elemType n)
{
    int borrow = 0;
    auto res = std::make_shared<magVec>(v1.size());
    auto & r = *res;
    elemType s1;
    r[0] = v1[0] - n;
    borrow = v1[0] < n;
    for (std::size_t i = 1; i < v1.size() && borrow != 0; i++)
    {
        s1 = v1[i] - borrow;
        borrow = s1 == elemMAX;
        r[i] = s1;
    }
    return res;
}

BigInteger::magType BigInteger::mul(const magVec & v1, const magVec & v2)
{
    return longMul(v1, v2);
}

BigInteger::magType BigInteger::longMul(const magVec & a, const magVec & b)
{
    swap_and_define 
    auto res = std::make_shared<magVec>(maxSize + minSize); 
    auto & r = *res;
    largeType temp;
    elemType carry = 0;
    for (std::size_t i = 0; i < minSize; ++i)
    {
        for (std::size_t j = 0; j < maxSize; ++j)
        {
            temp = static_cast<largeType>(v1[j]) * v2[i] + carry + r[i + j];
            carry = temp >> elemWIDTH;
            r[i + j] = temp;
        }
        r[i + maxSize] = carry;
        carry = 0;
    }
    if (r.back() == 0)
        r.resize(r.size() - 1);
    return res;
}

BigInteger::magType BigInteger::longMul(const magVec & v1, const elemType n)
{
    auto res = std::make_shared<magVec>(v1.size() + 1);
    auto & r = *res;
    largeType temp;
    elemType carry = 0;
    for (std::size_t i = 0; i < v1.size(); ++i)
    {
        temp = static_cast<largeType>(n) * v1[i] + carry;
        carry = temp >> elemWIDTH;
        r[i] = static_cast<elemType>(temp);
    }
    if (carry == 0)
        r.resize(v1.size());
    else
        r.back() = carry;
    return res;
}

BigInteger::twoElemType BigInteger::mullh(const elemType a, const elemType b)
{
    elemType h, l;
    asm volatile
    (
        "mulq %2"
            : "=a"(l), "=d"(h)
            : "%r"(a), "d"(b)
    );
    return {h, l};
}

BigInteger::twoElemType BigInteger::divrem(const elemType h, const elemType l, 
    const elemType d)
{
    elemType q, r;
    asm volatile
    (
        "divq %2"
            : "=a"(q), "=d"(r)
            : "r"(d), "a"(l), "d"(h)
    );
    return {q, r};
}

int BigInteger::normalize(magVec & v)
{
    largeType t;
    elemType temp = 0;
    int count = __builtin_clzl(v.back());
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        t = v[i];
        t <<= count;
        v[i] = t | temp;
        temp = t >> elemWIDTH;
    }
    return count;
}

void BigInteger::subMutable(magVec & u, const magVec & s, const std::size_t index)
{
    int borrow = 0, br1, br2;
    elemType s1, s2;
    for (std::size_t i = 0, j = index; i < s.size(); ++i, ++j)
    {
        s1 = u[j] - s[i];
        br1 = s1 > u[j];
        s2 = s1 - borrow;
        br2 = s2 == elemMAX;
        borrow = br1 | br2;
        u[j] = s2;
    }
    if (borrow)
        throw BigIntegerException("unexpected borrow");
}

/**
 * @brief helper function for knuth's division algorithm D4
 * 
 * @param q test quotient
 * @param index index j, substract start at index j of u
 * @return int return 1 if no borrow, 0 if borrow
 */
int BigInteger::subMul(magVec & u, const magVec & v, const elemType q,const std::size_t index)
{
    if (q == 0)
        return 1;

    if (q > 1)
    {
        // substract one less time
        auto sp = longMul(v, q - 1);
        auto & s = *sp;
        subMutable(u, s, index);
    }

    // D5
    // test if the last substraction will cause borrow
    if (compare(u, v) < 0)
        return 0;
    else
    {
        subMutable(u, v, index);
        return 1;
    }
}

void BigInteger::divFull(const magVec & a, const magVec & b, magVec & q, magVec & r, bool remainder)
{
    std::size_t n = b.size();
    std::size_t m = a.size() - n;

    bool pass;
    elemType quo, rem, t; // test quotient, remainder and a temp var
    elemType high, low, divisor;
    auto v = b;
    // D1 normalize
    int count = normalize(v);
    auto up = sl(a, count);
    auto u = *up;
    // D2 loop
    for (std::size_t j = m; j >= 0; --j)
    {
        high = u[n + j];
        low = u[n - 1 + j];
        divisor = v[n - 1];

        // D3
        assert(high <= divisor);
        if (high == divisor)
        {
            // quo == base condition
            // do the substraction and addtion
            quo = elemMAX;
            rem = low + divisor;
            if (rem < low && rem > 0) // !(r < base)
                pass = true;
            else
                pass = false;
        }
        else
            std::tie(quo, rem) = divrem(high, low, divisor);
        while (!pass && (mullh(quo, v[n - 2]) > std::make_tuple(rem, u[n - 2 + j])))
        {
            --quo;
            t = rem + divisor;
            if (t < rem && t > 0) // !(r < base)
                break;
            else
                rem = t;
        }
        // D4
        if (!subMul(u, v, quo, j))
            // D6, no need for add back, handled in subMul
            ++quo;
        q[j] = quo;
    }
    // get remainder
    if (remainder)
    {
        largeType temp;
        temp = static_cast<largeType>(u[0]) << elemWIDTH - count;
        r[0] = static_cast<elemType>(temp >> elemWIDTH);
        // remainder's size cannot be large than divisor
        for (std::size_t i = 1; i < b.size(); ++i) 
        {
            if (u[i] == 0)
                break;
            temp = static_cast<largeType>(u[i]) << elemWIDTH - count;
            r[i] = static_cast<elemType>(temp >> elemWIDTH);
            r[i - 1] |= static_cast<elemType>(temp);
        }
    }
}

BigInteger::magType BigInteger::div(const magVec & a, const magVec & b)
{
    auto res = std::make_shared<magVec>(a.size() - b.size() + 1);
    auto & r = *res;
    divFull(a, b, r, r, false);
    return res;
}

std::tuple<BigInteger::magType, BigInteger::magType> BigInteger::divrem(const magVec & a, const magVec & b)
{
    auto quo = std::make_shared<magVec>(a.size() - b.size() + 1);
    auto & q = *quo;
    auto rem = std::make_shared<magVec>(b.size());
    auto & r = *rem;
    divFull(a, b, q, r, true);
    return {quo, rem};
}

BigInteger::magType BigInteger::div(const magVec & v, const elemType n)
{
    auto res = std::make_shared<magVec>(v.size());
    auto & r = *res;
    elemType temp = v.back();
    for (std::size_t i = v.size() - 2; i >= 0; --i)
    {
        if (temp >= n)
        {
            r[i + 1] = temp / n;
            temp %= n;
        }
        std::tie(r[i], temp) = divrem(temp, v[i], n);
    }
    return res;
}


BigInteger::magType BigInteger::andFunc(const magVec & a, const magVec & b) 
{
    swap_and_define
    // 111 & ~110101 = 111 & ~101
    auto res = std::make_shared<magVec>(minSize);
    auto & r = *res;
    std::size_t i = 0, t;
    auto count = v2.size();
    std::generate_n(r.begin(), count, [&] () {
        t = i; ++i;
        return v1[t] & v2[t];
    });
    return res;
}
BigInteger::magType BigInteger::andnot(const magVec & v1, const magVec & v2) 
{
    auto maxSize = std::max(v1.size(), v2.size());
    auto minSize = std::min(v1.size(), v2.size());
    // 111 & ~110101 = 111 & ~101
    auto res = std::make_shared<magVec>(minSize); 
    auto & r = *res;
    std::size_t i = 0, t;
    std::generate_n(r.begin(), minSize, [&] () {
        t = i; ++i;
        return v1[t] & ~v2[t];
    });
    if (v1.size() > v2.size())
    {
        // 110101 & ~111 = 110101 & 111000
        r.resize(maxSize);
        for (; i < maxSize; ++i)
            r[i] = v1[i];
    }
    return res;
}
BigInteger::magType BigInteger::orFunc(const magVec & a, const magVec & b)
{
    swap_and_define
    auto res = std::make_shared<magVec>(maxSize); 
    auto & r = *res;
    std::size_t i = 0, t;
    auto count = v2.size();
    std::generate_n(r.begin(), count, [&] () {
        t = i; ++i;
        return v1[t] | v2[t];
    });
    for (; i < maxSize; ++i)
        r[i] = v1[i];
    return res;
}
BigInteger::magType BigInteger::xorFunc(const magVec & a, const magVec & b)
{
    swap_and_define
    auto res = std::make_shared<magVec>(maxSize); 
    auto & r = *res;
    std::size_t i = 0, t;
    auto count = v2.size();
    std::generate_n(r.begin(), count, [&] () {
        t = i; ++i;
        return v1[t] ^ v2[t];
    });
    for (; i < maxSize; ++i)
        r[i] = v1[i];
    return res;
}

BigInteger::magType BigInteger::sl(const magVec & v, const elemType n)
{
    largeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    auto res = std::make_shared<magVec>(v.size() + count + 1);
    auto & r = *res;
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        t = v[i];
        t <<= rem;
        r[count + i] |= t;
        r[count + i + 1] = t >> elemWIDTH;
    }
    if (r.back() == 0)
        r.resize(r.size() - 1);
    return res;
}
BigInteger::magType BigInteger::sr(const magVec & v, const elemType n)
{
    largeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    assert(count < v.size());
    auto res = std::make_shared<magVec>(v.size() - count);
    auto & r = *res;
    largeType t;
    t = static_cast<largeType>(v[count]) << elemWIDTH - rem;
    r[0] = static_cast<elemType>(t >> elemWIDTH);
    for (std::size_t i = 1 + count; i < v.size(); ++i) 
    {
        t = static_cast<largeType>(v[i]) << elemWIDTH - rem;
        r[i - count] = static_cast<elemType>(t >> elemWIDTH);
        r[i - count - 1] |= static_cast<elemType>(t);
    }
    if (r.back() == 0)
        r.resize(r.size() - 1);
    return res;
}

int BigInteger::convert(magVec & v, const char * str, int base)
{
    int step = digitsPerInt[base - 2];
    elemType value = 0;
    bool start = false;
    char last = ' ';
    int sign = 0;
    for (; *str != 0; str++)
    {
        if (!start)
        {
            // get rid of leading 0 and blank
            if (std::isblank(*str) || *str == '0')
                continue;
            else 
            {
                start = true;
                if (*str == '-')
                    sign = -1;
                else if (*str == '+')
                    sign = 1;
                else if (digit(*str, base) != -1)
                {
                    sign = 1;
                    str--;
                }
                continue;
            }
        }
        else
        {
            int v = digit(*str, base);
            if (v == -1)
                throw BigIntegerException("Invalid character");
            
        }
    }
    return sign;
}

std::string BigInteger::toString(int base) const
{
    ostringstream s;
    if (base == 10)
        for (auto && iter = magnitude->crbegin(); iter != magnitude->crend(); iter++)
            s << *iter;
    return s.str();
}