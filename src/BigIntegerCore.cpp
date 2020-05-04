#include "BigInteger.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace zyd2001;

// initialize order is important, otherwise zero will have null ptr
const BigInteger::VecPtr BigInteger::zeroPtr = std::make_shared<Vec>();
const BigInteger BigInteger::zero = BigInteger();

int BigInteger::compare(const Vec & v1, const Vec & v2)
{
    if (v1.size() > v2.size())
        return 1;
    else if (v1.size() < v2.size())
        return -1;
    else
    {
        ElemType t1, t2;
        for (std::size_t i = v1.size(); i > 0; i--)
        {
            t1 = v1[i - 1];
            t2 = v2[i - 1];
            if (t1 != t2)
                return (t1 > t2) ? 1 : -1;
        }
        return 0;
    }
}

void BigInteger::removeZero(Vec & v)
{
    std::size_t i = v.size();
    for (; i > 0; i--)
        if (v[i - 1] != 0)
            break;
    v.resize(i);
}

// swap two arguments to make sure v1 is large one
#define swap_and_define                        \
    auto & v1 = (a.size() < b.size()) ? b : a; \
    auto & v2 = (a.size() < b.size()) ? a : b; \
    auto maxSize = v1.size();                  \
    auto minSize = v2.size();

BigInteger::VecPtr BigInteger::add(const Vec & a, const Vec & b)
{
    int carry = 0, ca1, ca2;
    swap_and_define;
    auto res = std::make_shared<Vec>(maxSize + 1);
    auto & r = *res;
    ElemType s1, s2, t;
    for (std::size_t i = 0; i < minSize; i++)
    {
        t = v1[i];
        s1 = t + v2[i];
        ca1 = s1 < t;
        s2 = s1 + carry;
        ca2 = s2 < s1;
        carry = ca1 | ca2;
        r[i] = s2;
    }
    for (std::size_t i = minSize; i < maxSize; i++)
    {
        t = v1[i];
        s1 = t + carry;
        carry = s1 < t;
        r[i] = s1;
    }
    if (carry)
        r.back() = 1;
    else
        r.resize(maxSize);
    return res;
}

BigInteger::VecPtr BigInteger::add(const Vec & v1, const ElemType n)
{
    assert(v1.size() > 0);
    int carry = 0;
    auto res = std::make_shared<Vec>(v1.size() + 1);
    auto & r = *res;
    ElemType sl, t;
    sl = v1[0] + n;
    carry = sl < v1[0];
    r[0] = sl;
    for (std::size_t i = 1; i < v1.size(); i++)
    {
        t = v1[i];
        sl = t + carry;
        carry = sl < t;
        r[i] = sl;
    }
    if (carry)
        r.back() = 1;
    else
        r.resize(v1.size());
    return res;
}

// TODO: last nonzero
// assume v1 > v2
BigInteger::VecPtr BigInteger::sub(const Vec & v1, const Vec & v2)
{
    assert(compare(v1, v2) > 0);
    int borrow = 0, br1, br2;
    auto res = std::make_shared<Vec>(v1.size());
    auto & r = *res;
    ElemType s1, s2, t;
    for (std::size_t i = 0; i < v2.size(); i++)
    {
        // if overflow, the result will just be uintmax + v1[i] - v[2]
        // because of 2's complement
        t = v1[i];
        s1 = t - v2[i];
        br1 = s1 > t;
        s2 = s1 - borrow;
        br2 = s2 > s1;
        borrow = br1 | br2;
        r[i] = s2;
    }
    for (std::size_t i = v2.size(); i < v1.size(); i++)
    {
        t = v1[i];
        s1 = t - borrow;
        borrow = s1 > t;
        r[i] = s1;
    }
    removeZero(r);
    return res;
}

// assume v1 >= n
BigInteger::VecPtr BigInteger::sub(const Vec & v1, const ElemType n)
{
    assert(v1.size() > 1 || (v1.size() == 1 && v1[0] >= n));
    int borrow = 0;
    auto res = std::make_shared<Vec>(v1.size());
    auto & r = *res;
    ElemType s1, t;
    r[0] = v1[0] - n;
    borrow = v1[0] < n;
    for (std::size_t i = 1; i < v1.size(); i++)
    {
        t = v1[i];
        s1 = t - borrow;
        borrow = s1 > t;
        r[i] = s1;
    }
    removeZero(r);
    return res;
}

BigInteger::VecPtr BigInteger::mul(const Vec & v1, const Vec & v2) { return longMul(v1, v2); }

BigInteger::VecPtr BigInteger::longMul(const Vec & a, const Vec & b)
{
    swap_and_define;
    auto res = std::make_shared<Vec>(maxSize + minSize);
    auto & r = *res;
    LargeType temp;
    ElemType carry = 0;
    for (std::size_t i = 0; i < minSize; i++)
    {
        for (std::size_t j = 0; j < maxSize; j++)
        {
            temp = static_cast<LargeType>(v1[j]) * v2[i] + carry + r[i + j];
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

BigInteger::VecPtr BigInteger::longMul(const Vec & v1, const ElemType n)
{
    auto res = std::make_shared<Vec>(v1.size() + 1);
    auto & r = *res;
    LargeType temp;
    ElemType carry = 0;
    for (std::size_t i = 0; i < v1.size(); i++)
    {
        temp = static_cast<LargeType>(n) * v1[i] + carry;
        carry = temp >> elemWIDTH;
        r[i] = static_cast<ElemType>(temp);
    }
    if (carry == 0)
        r.resize(v1.size());
    else
        r.back() = carry;
    return res;
}

// TODO: generic
// clang-format off
BigInteger::TwoElemType BigInteger::mullh(const ElemType a, const ElemType b)
{
    ElemType h, l;
    asm volatile
    (
        "mulq %2"
            : "=a"(l), "=d"(h)
            : "%r"(a), "d"(b)
    );
    return {h, l};
}

BigInteger::TwoElemType BigInteger::divrem(const ElemType h, const ElemType l, 
    const ElemType d)
{
    ElemType q, r;
    asm volatile
    (
        "divq %2"
            : "=a"(q), "=d"(r)
            : "r"(d), "a"(l), "d"(h)
    );
    return {q, r};
}
// clang-format on

int BigInteger::normalize(Vec & v)
{
    LargeType t;
    ElemType temp = 0;
    int count = __builtin_clzl(v.back());
    for (std::size_t i = 0; i < v.size(); i++)
    {
        t = v[i];
        t <<= count;
        v[i] = t | temp;
        temp = t >> elemWIDTH;
    }
    return count;
}

void BigInteger::subMutable(Vec & u, const Vec & s, const std::size_t index)
{
    int borrow = 0, br1, br2;
    ElemType s1, s2;
    for (std::size_t i = 0, j = index; i < s.size(); i++, j++)
    {
        s1 = u[j] - s[i];
        br1 = s1 > u[j];
        s2 = s1 - borrow;
        br2 = s2 == elemMAX;
        borrow = br1 | br2;
        u[j] = s2;
    }
    assert(!borrow);
}

/**
 * @brief helper function for knuth's division algorithm D4
 *
 * @param q test quotient
 * @param index index j, substract start at index j of u
 * @return int return 1 if no borrow, 0 if borrow
 */
int BigInteger::subMul(Vec & u, const Vec & v, const ElemType q, const std::size_t index)
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

void BigInteger::divFull(const Vec & a, const Vec & b, Vec & q, Vec & r, bool remainder)
{
    std::size_t n = b.size();
    std::size_t m = a.size() - n;

    bool pass;
    ElemType quo, rem, t; // test quotient, remainder and a temp var
    ElemType high, low, divisor;
    auto v = b;
    // D1 normalize
    int count = normalize(v);
    auto up = sl(a, count);
    auto u = *up;
    // D2 loop
    for (std::size_t j = m; j >= 0; j--)
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
        LargeType temp;
        temp = static_cast<LargeType>(u[0]) << (elemWIDTH - count);
        r[0] = static_cast<ElemType>(temp >> elemWIDTH);
        // remainder's size cannot be large than divisor
        for (std::size_t i = 1; i < b.size(); i++)
        {
            if (u[i] == 0)
                break;
            temp = static_cast<LargeType>(u[i]) << (elemWIDTH - count);
            r[i] = static_cast<ElemType>(temp >> elemWIDTH);
            r[i - 1] |= static_cast<ElemType>(temp);
        }
    }
}

BigInteger::VecPtr BigInteger::div(const Vec & a, const Vec & b)
{
    auto res = std::make_shared<Vec>(a.size() - b.size() + 1);
    auto & r = *res;
    divFull(a, b, r, r, false);
    if (r.back() == 0)
        r.resize(a.size() - b.size());
    return res;
}

std::tuple<BigInteger::VecPtr, BigInteger::VecPtr> BigInteger::divrem(const Vec & a, const Vec & b)
{
    auto quo = std::make_shared<Vec>(a.size() - b.size() + 1);
    auto & q = *quo;
    auto rem = std::make_shared<Vec>(b.size());
    auto & r = *rem;
    divFull(a, b, q, r, true);
    if (q.back() == 0)
        q.resize(a.size() - b.size());
    // remainder comes from u be substracted several times, so removeZero
    removeZero(r);
    return {quo, rem};
}

std::tuple<BigInteger::VecPtr, BigInteger::ElemType> BigInteger::divrem(const Vec & v, const ElemType n)
{
    VecPtr res;
    ElemType temp = v.back();
    // if first high elem isn't larger that n, r size is v.size() - 1
    if (temp >= n)
    {
        res = std::make_shared<Vec>(v.size());
        res->back() = temp / n;
        temp = temp % n;
    }
    else
        res = std::make_shared<Vec>(v.size() - 1);
    auto & r = *res;
    for (std::size_t i = v.size() - 1; i > 0; i--)
        std::tie(r[i - 1], temp) = divrem(temp, v[i - 1], n);
    return {res, temp};
}

BigInteger::VecPtr BigInteger::div(const Vec & v, const ElemType n) { return std::get<0>(divrem(v, n)); }

BigInteger::VecPtr BigInteger::andFunc(const Vec & a, const Vec & b)
{
    swap_and_define;
    // 111 & 110101 = 000111 & 110101
    auto res = std::make_shared<Vec>(minSize);
    auto & r = *res;
    std::size_t i = 0, t;
    std::generate_n(r.begin(), minSize, [&]() {
        t = i;
        i++;
        return v1[t] & v2[t];
    });
    return res;
}
BigInteger::VecPtr BigInteger::andnot(const Vec & v1, const Vec & v2)
{
    auto maxSize = std::max(v1.size(), v2.size());
    auto minSize = std::min(v1.size(), v2.size());
    // 111 & ~110101 = 000111 & ~110101
    auto res = std::make_shared<Vec>(minSize);
    auto & r = *res;
    std::size_t i = 0, t;
    std::generate_n(r.begin(), minSize, [&]() {
        t = i;
        i++;
        return v1[t] & ~v2[t];
    });
    if (v1.size() > v2.size())
    {
        // 110101 & ~111 = 110101 & 111000
        r.resize(maxSize);
        for (; i < maxSize; i++)
            r[i] = v1[i];
    }
    return res;
}
BigInteger::VecPtr BigInteger::orFunc(const Vec & a, const Vec & b)
{
    swap_and_define;
    auto res = std::make_shared<Vec>(maxSize);
    auto & r = *res;
    std::size_t i = 0, t;
    auto count = v2.size();
    std::generate_n(r.begin(), count, [&]() {
        t = i;
        i++;
        return v1[t] | v2[t];
    });
    for (; i < maxSize; i++)
        r[i] = v1[i];
    return res;
}
BigInteger::VecPtr BigInteger::xorFunc(const Vec & a, const Vec & b)
{
    swap_and_define;
    auto res = std::make_shared<Vec>(maxSize);
    auto & r = *res;
    std::size_t i = 0, t;
    auto count = v2.size();
    std::generate_n(r.begin(), count, [&]() {
        t = i;
        i++;
        return v1[t] ^ v2[t];
    });
    for (; i < maxSize; i++)
        r[i] = v1[i];
    return res;
}

BigInteger::VecPtr BigInteger::sl(const Vec & v, const ElemType n)
{
    LargeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    auto res = std::make_shared<Vec>(v.size() + count + 1);
    auto & r = *res;
    for (std::size_t i = 0; i < v.size(); i++)
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
BigInteger::VecPtr BigInteger::sr(const Vec & v, const ElemType n)
{
    LargeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    assert(count < v.size());
    auto res = std::make_shared<Vec>(v.size() - count);
    auto & r = *res;
    t = static_cast<LargeType>(v[count]) << (elemWIDTH - rem);
    r[0] = static_cast<ElemType>(t >> elemWIDTH);
    for (std::size_t i = 1 + count; i < v.size(); i++)
    {
        t = static_cast<LargeType>(v[i]) << (elemWIDTH - rem);
        r[i - count] = static_cast<ElemType>(t >> elemWIDTH);
        r[i - count - 1] |= static_cast<ElemType>(t);
    }
    if (r.back() == 0)
        r.resize(r.size() - 1);
    return res;
}

// TODO: Type
const int BigInteger::digitsPerElem[36] = {0, 0, 63, 40, 31, 27, 24, 22, 21, 20, 19, 18, 17, 17, 16, 16, 15, 15,
    15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12};
const BigInteger::ElemType BigInteger::maxPerElem[36] = {1, 1, 9223372036854775808UL, 12157665459056928801UL,
    4611686018427387904UL, 7450580596923828125UL, 4738381338321616896UL, 3909821048582988049UL,
    9223372036854775808UL, 12157665459056928801UL, 10000000000000000000UL, 5559917313492231481UL,
    2218611106740436992UL, 8650415919381337933UL, 2177953337809371136UL, 6568408355712890625UL,
    1152921504606846976UL, 2862423051509815793UL, 6746640616477458432UL, 15181127029874798299UL,
    1638400000000000000UL, 3243919932521508681UL, 6221821273427820544UL, 11592836324538749809UL,
    876488338465357824UL, 1490116119384765625UL, 2481152873203736576UL, 4052555153018976267UL,
    6502111422497947648UL, 10260628712958602189UL, 15943230000000000000UL, 787662783788549761UL,
    1152921504606846976UL, 1667889514952984961UL, 2386420683693101056UL};

/**
 * @brief helper Function to convert char to int
 *
 * @param ch
 * @param base
 * @throw BigIntegerException when ch is invalid
 */
int BigInteger::digit(char ch, int base)
{
    if (base > 10)
    {
        if (ch >= '0' && ch <= '9')
            return ch - '0';
        else if (ch >= 'a' && ch < 'a' + base - 10)
            return ch - 'a' + 10;
        else
            throw BigIntegerException("Invalid character");
    }
    else
    {
        if (ch >= '0' && ch < '0' + base)
        {
            return ch - '0';
        }
        else
            throw BigIntegerException("Invalid character");
    }
}

BigInteger::ElemType BigInteger::convert(const char * str, int base, int length)
{
    ElemType num = 0;
    for (int i = 0; i < length; i++)
    {
        num *= base;
        num += digit(str[i], base);
    }
    return num;
}

void BigInteger::addMul(Vec & v, const ElemType p, const ElemType n)
{
    LargeType temp;
    ElemType carry = 0;
    if (v.size() == 0)
    {
        v.emplace_back(n);
        return;
    }
    for (std::size_t i = 0; i < v.size(); i++)
    {
        temp = static_cast<LargeType>(p) * v[i] + carry;
        carry = temp >> elemWIDTH;
        v[i] = static_cast<ElemType>(temp);
    }
    if (carry)
        v.emplace_back(carry);
    ElemType sl;
    sl = v[0] + n;
    carry = sl < v[0];
    v[0] = sl;
    for (std::size_t i = 1; i < v.size() && carry != 0; i++)
    {
        sl = v[i] + carry;
        carry = sl == 0; // if overflow, only possibility is 0
        v[i] = sl;
    }
    if (carry)
        v.emplace_back(carry);
}

int BigInteger::convert(Vec & v, const char * str, int base)
{
    ElemType value = 0;
    int sign = 1;
    int step = digitsPerElem[base];
    ElemType max = maxPerElem[base];
    char ch;

    // get rid of leading 0 and blank
    while (std::isspace(*str))
        str++;
    ch = *str;
    if (ch == '+')
        str++;
    else if (ch == '-')
    {
        str++;
        sign = -1;
    }
    while (*str == '0')
        str++;

    int i = 0;
    ch = *str;
    if (ch == 0 || std::isspace(ch))
        return 0;
    do // TODO: optimization
    {
        // i++;
        str++;
        addMul(v, base, digit(std::tolower(ch), base));
        // value *= base;
        // value += digit(ch, base);
        // if (i == step)
        // {
        //     addMul(v, max, value);
        //     value = 0;
        //     i = 0;
        // }
        ch = *str;
    } while (ch != 0 && !std::isspace(ch));
    // if (value != 0)
    //     addMul(v, max, value);
    return sign;
}

BigInteger::ElemType BigInteger::divremMutable(Vec & v, const ElemType n)
{
    ElemType temp = v.back();
    v.back() = temp / n;
    temp %= n;
    for (std::size_t i = v.size() - 1; i > 0; i--)
        std::tie(v[i - 1], temp) = divrem(temp, v[i - 1], n);
    if (v.back() == 0)
        v.resize(v.size() - 1);
    return temp;
}

char BigInteger::toChar(ElemType e, int base)
{
    assert(e < base);
    if (e < 10)
        return '0' + e;
    else
        return 'a' + e - 10;
}

std::string BigInteger::toString(int base, bool upper) const
{
    if (base < 2 || base > 36)
        throw BigIntegerException("Invalid base");
    if (this->sign == 0)
        return "0";
    std::ostringstream s;
    VecPtr ptr;
    Vec v = *this->magnitude;
    ElemType rem;
    char ch;
    while (v.size() != 0)
    {
        rem = divremMutable(v, base);
        ch = toChar(rem, base);
        if (upper)
            ch = toupper(ch);
        s << ch;
    }
    if (sign == -1)
        s << '-';
    std::string str = s.str();
    std::reverse(str.begin(), str.end());
    return str;
}