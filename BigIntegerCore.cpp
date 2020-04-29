#include "BigInteger.hpp"
#include <cctype>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>

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

int BigInteger::compare(const std::vector<elemType> & v1, const std::vector<elemType> & v2)
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

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::add(const std::vector<elemType> & a, 
    const std::vector<elemType> & b)
{
    int carry = 0;
    swap_and_define
    auto res = std::make_shared<std::vector<elemType>>(maxSize + 1);
    auto & r = *res;
    for (std::size_t i = 0; i < minSize; ++i)
    {
        r[i] = v1[i] + v2[i] + carry;
        carry = (r[i] < v1[i] || (r[i] == v1[i] && v2[i] != 0)) ? 1 : 0;
    }
    for (std::size_t i = minSize; i < maxSize && carry != 0; ++i)
    {
        r[i] = v1[i] + carry;
        carry = (r[i] == 0) ? 1 : 0; // if overflow, only possibility is 0
    }
    if (carry)
        r[maxSize] = 1;
    else
        r.resize(maxSize);
    return res;
}

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::add(const std::vector<elemType> & v1, 
    const elemType n)
{
    int carry = 0;
    auto res = std::make_shared<std::vector<elemType>>(v1.size() + 1);
    auto & r = *res;
    r[0] = v1[0] + n;
    carry = (r[0] < v1[0]) ? 1 : 0;
    for (std::size_t i = 1; i < v1.size() && carry != 0; i++)
    {
        r[i] = v1[i] + carry;
        carry = (r[i] == 0) ? 1 : 0; // if overflow, only possibility is 0
    }
    if (carry)
        r.back() = 1;
    else 
        r.resize(v1.size());
    return res;
}

// assume v1 > v2
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::sub(const std::vector<elemType> & v1, 
    const std::vector<elemType> & v2)
{
    int borrow = 0;
    auto res = std::make_shared<std::vector<elemType>>(v1.size());
    auto & r = *res;
    for (std::size_t i = 0; i < v2.size(); ++i)
    {
        // if overflow, the result will just be uintmax + v1[i] - v[2]
        // because of 2's complement
        // r[i] = v1[i] - v2[i] - borrow;
        // if (v1[i] > v2[i])
        //     borrow = 0;
        // else
        //     borrow = (borrow || (v1[i] < v2[i])) ? 1 : 0;
        r[i] = v1[i] - v2[i] - borrow;
        if (r[i] > v1[i] || (r[i] == v1[i] && v2[i] != 0))
            borrow = 1;
        else
            borrow = 0;
    }
    for (std::size_t i = v2.size(); i < v1.size() && borrow != 0; ++i)
    {
        r[i] = v1[i] - borrow;
        borrow = (r[i] == elemMAX) ? 1 : 0;
    }
    return res;
}

// assume v1 > n
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::sub(const std::vector<elemType> & v1, 
    const elemType n)
{
    int borrow = 0;
    auto res = std::make_shared<std::vector<elemType>>(v1.size());
    auto & r = *res;
    r[0] = v1[0] - n;
    borrow = (v1[0] < n) ? 1 : 0;
    for (std::size_t i = 1; i < v1.size() && borrow != 0; i++)
    {
        r[i] = v1[i] - borrow;
        borrow = (r[i] == elemMAX) ? 1 : 0;
    }
    return res;
}

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::mul(const std::vector<elemType> & v1, 
    const std::vector<elemType> & v2)
{
    return longMul(v1, v2);
}

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::longMul(const std::vector<elemType> & a, 
    const std::vector<elemType> & b)
{
    swap_and_define 
    auto res = std::make_shared<std::vector<elemType>>(maxSize + minSize); 
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

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::longMul(const std::vector<elemType> & v1, 
    const elemType n)
{
    auto res = std::make_shared<std::vector<elemType>>(v1.size() + 1);
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

std::tuple<BigInteger::elemType, BigInteger::elemType> BigInteger::mullh(const elemType a, const elemType b)
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

std::tuple<BigInteger::elemType, BigInteger::elemType> BigInteger::divrem(const elemType h, const elemType l, 
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

void BigInteger::subMutable(std::vector<elemType> & u, const std::vector<elemType> & s)
{
    elemType borrow = 0, ut;
    for (std::size_t i = 0; i < s.size(); ++i)
    {
        ut = u[i];
        u[i] = u[i] - s[i] - borrow;
        if (u[i] >= ut || (u[i] == ut && s[i] != 0))
            borrow = 1;
        else
            borrow = 0;
    }
    for (std::size_t i = s.size(); i < u.size() && borrow != 0; ++i)
    {
        u[i] = u[i] - borrow;
        borrow = (u[i] == elemMAX) ? 1 : 0;
    }
}

int BigInteger::subMul(std::vector<elemType> & u, const std::vector<elemType> & v, const elemType q)
{
    if (q == 0)
        return 1;

    if (q > 1)
    {
        auto sp = longMul(v, q - 1);
        auto & s = *sp;
        subMutable(u, s);
    }

    if (compare(u, v) < 0)
        return 0;
    else
    {
        subMutable(u, v);
        return 1;
    }
}

// TODO: trim leading zero
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::div(const std::vector<elemType> & a, 
    const std::vector<elemType> & b)
{
    std::size_t n = b.size();
    std::size_t m = a.size() - n;
    auto res = std::make_shared<std::vector<elemType>>(m + 1);
    auto & r = *res;
    elemType _q, _r; // test quotient, remainder
    auto v = b;
    // D1 normalize
    int count = sl(v);
    auto up = sl(a, count);
    auto u = *up;
    // loop
    for (std::size_t j = m; j >= 0; --j)
    {
        // D3
        std::tie(_q, _r) = divrem(u[n + j], u[n - 1 + j], v[n - 1]);
        while (mullh(_q, v[n - 2]) > std::make_tuple(_r, u[n - 2 + j]))
        {
            --_q;
            largeType t = static_cast<largeType>(_r) + v[n - 1];
            if (t > elemMAX) // r >= base
                break;
            else
                _r = static_cast<elemType>(t);
        }
        if (!subMul(u, v, _q))
            // D6, no need for add back, handled in subMul
            ++_q;
        r[j] = _q;
    }
    return res;
}

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::div(const std::vector<elemType> & v, 
    const elemType n)
{
    auto res = std::make_shared<std::vector<elemType>>(v.size());
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


std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::andFunc(const std::vector<elemType> & a, 
    const std::vector<elemType> & b) 
{
    swap_and_define
    // 111 & ~110101 = 111 & ~101
    auto res = std::make_shared<std::vector<elemType>>(minSize);
    auto & r = *res;
    std::size_t i = 0, t;
    auto count = v2.size();
    std::generate_n(r.begin(), count, [&] () {
        t = i; ++i;
        return v1[t] & v2[t];
    });
    return res;
}
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::andnot(const std::vector<elemType> & v1, 
    const std::vector<elemType> & v2) 
{
    auto maxSize = std::max(v1.size(), v2.size());
    auto minSize = std::min(v1.size(), v2.size());
    // 111 & ~110101 = 111 & ~101
    auto res = std::make_shared<std::vector<elemType>>(minSize); 
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
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::orFunc(const std::vector<elemType> & a, 
    const std::vector<elemType> & b)
{
    swap_and_define
    auto res = std::make_shared<std::vector<elemType>>(maxSize); 
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
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::xorFunc(const std::vector<elemType> & a, 
    const std::vector<elemType> & b)
{
    swap_and_define
    auto res = std::make_shared<std::vector<elemType>>(maxSize); 
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

int BigInteger::sl(std::vector<elemType> & v)
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

std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::sl(const std::vector<elemType> & v, const elemType n)
{
    largeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    auto res = std::make_shared<std::vector<elemType>>(v.size() + count + 1);
    auto & r = *res;
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        t = v[i];
        t <<= rem;
        r[count + i] |= t;
        r[count + i + 1] = t >> elemWIDTH;
    }
    return res;
}
std::shared_ptr<std::vector<BigInteger::elemType>> BigInteger::sr(const std::vector<elemType> & v, const elemType n)
{
    largeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    auto res = std::make_shared<std::vector<elemType>>(v.size() - count);
    auto & r = *res;
}

int BigInteger::convert(std::vector<elemType> & v, const char * str, int base)
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