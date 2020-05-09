#include "BigInteger/BigInteger.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <sstream>
#include <utility>

using namespace zyd2001;
#define Mag BigInteger::Magnitude

Mag::Magnitude(std::size_t i) : vec(std::make_unique<ElemType[]>(i)), size(i) {}

std::unique_ptr<BigInteger::ElemType[]> Mag::copy() const
{
    auto ptr = std::make_unique<ElemType[]>(size);
    // std::copy_n(this->begin(), size, ptr.get());
    for (std::size_t i = 0; i < size; i++)
        ptr[i] = (*this)[i];
    return ptr;
}

int Mag::compare(const Mag & v2) const
{
    auto && v1 = *this;
    if (v1.size > v2.size)
        return 1;
    else if (v1.size < v2.size)
        return -1;
    else
    {
        ElemType t1, t2;
        for (std::size_t i = v1.size; i > 0; i--)
        {
            t1 = v1[i - 1];
            t2 = v2[i - 1];
            if (t1 != t2)
                return (t1 > t2) ? 1 : -1;
        }
        return 0;
    }
}

void Mag::removeZero()
{
    auto && v = *this;
    std::size_t i = v.size;
    for (; i > 0; i--)
        if (v[i - 1] != 0)
            break;
    v.resize(i);
}

// swap two arguments to make sure v1 is large one
#define swap_and_define                    \
    auto & v1 = (a.size < b.size) ? b : a; \
    auto & v2 = (a.size < b.size) ? a : b; \
    auto maxSize = v1.size;                \
    auto minSize = v2.size;

Mag Mag::operator+(const Mag & b) const
{
    auto && a = *this;
    int carry = 0, ca1, ca2;
    swap_and_define;
    Mag r(maxSize + 1);
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
        r.size--;
    return r;
}

Mag Mag::operator+(const ElemType n) const
{
    auto && v1 = *this;
    assert(v1.size > 0);
    int carry = 0;
    Mag r(v1.size + 1);
    ElemType sl, t;
    sl = v1[0] + n;
    carry = sl < v1[0];
    r[0] = sl;
    for (std::size_t i = 1; i < v1.size; i++)
    {
        t = v1[i];
        sl = t + carry;
        carry = sl < t;
        r[i] = sl;
    }
    if (carry)
        r.back() = 1;
    else
        r.size--;
    return r;
}

// TODO: last nonzero
// assume v1 > v2
Mag Mag::operator-(const Mag & v2) const
{
    auto && v1 = *this;
    assert(v1.compare(v2) > 0);
    int borrow = 0, br1, br2;
    Mag r(v1.size);
    ElemType s1, s2, t;
    for (std::size_t i = 0; i < v2.size; i++)
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
    for (std::size_t i = v2.size; i < v1.size; i++)
    {
        t = v1[i];
        s1 = t - borrow;
        borrow = s1 > t;
        r[i] = s1;
    }
    r.removeZero();
    return r;
}

// assume v1 >= n
Mag Mag::operator-(const ElemType n) const
{
    auto && v1 = *this;
    assert(v1.size > 1 || (v1.size == 1 && v1[0] >= n));
    int borrow = 0;
    Mag r(v1.size);
    ElemType s1, t;
    r[0] = v1[0] - n;
    borrow = v1[0] < n;
    for (std::size_t i = 1; i < v1.size; i++)
    {
        t = v1[i];
        s1 = t - borrow;
        borrow = s1 > t;
        r[i] = s1;
    }
    r.removeZero();
    return r;
}

Mag Mag::operator*(const Mag & v2) const { return this->longMul(v2); }

Mag Mag::longMul(const Mag & b) const
{
    auto && a = *this;
    swap_and_define;
    Mag r(maxSize + minSize);
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
        r.size--;
    return r;
}

Mag Mag::longMul(const ElemType n) const
{
    auto && v1 = *this;
    Mag r(v1.size + 1);
    LargeType temp;
    ElemType carry = 0;
    for (std::size_t i = 0; i < v1.size; i++)
    {
        temp = static_cast<LargeType>(n) * v1[i] + carry;
        carry = temp >> elemWIDTH;
        r[i] = static_cast<ElemType>(temp);
    }
    if (carry == 0)
        r.size--;
    else
        r.back() = carry;
    return r;
}

// TODO: generic
// clang-format off
Mag::TwoElemType Mag::mullh(const ElemType a, const ElemType b)
{
    ElemType h, l;
    asm volatile
    (
        "mulq %2"
            : "=a"(l), "=d"(h)
            : "%r"(a), "a"(b)
    );
    return {h, l};
}

Mag::TwoElemType Mag::divrem(const ElemType h, const ElemType l, const ElemType d)
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

int Mag::normalize()
{
    auto && v = *this;
    LargeType t;
    ElemType temp = 0;
    int count = __builtin_clzl(v.back());
    for (std::size_t i = 0; i < v.size; i++)
    {
        t = v[i];
        t <<= count;
        v[i] = t | temp;
        temp = t >> elemWIDTH;
    }
    return count;
}

void Mag::subMutable(const Mag & s, const std::size_t index)
{
    auto && u = *this;
    int borrow = 0, br1, br2;
    ElemType s1, s2, t;
    std::size_t n = s.size;
    for (std::size_t i = 0, j = index; i < n; i++, j++)
    {
        t = u[j];
        s1 = t - s[i];
        br1 = s1 > t;
        s2 = s1 - borrow;
        br2 = s2 > s1;
        borrow = br1 | br2;
        u[j] = s2;
    }
    if (borrow)
    {
        t = u[index + n];
        s1 = t - borrow;
        u[index + n] = s1;
        assert(!(s1 > t));
    }
}

/**
 * @brief helper function for knuth's division algorithm D4
 *
 * @param q test quotient
 * @param index index j, substract start at index j of u
 * @return int return 1 if no borrow, 0 if borrow
 */
int Mag::subMul(const Mag & v, const ElemType q, const std::size_t index)
{
    auto && u = *this;
    if (q == 0)
        return 1;

    if (q > 1)
    {
        // substract one less time
        auto && s = v.longMul(q - 1);
        u.subMutable(s, index);
    }

    // D5
    // test if the last substraction will cause borrow
    std::size_t n = v.size;
    ElemType t1, t2;
    if (u[index + n] == 0)
        for (std::size_t i = n; i > 0; i--)
        {
            t1 = u[i + index - 1];
            t2 = v[i - 1];
            if (t1 < t2)
                return 0;
            else if (t1 > t2)
                break;
            else
                continue;
        }
    u.subMutable(v, index);
    return 1;
}

void Mag::divFull(const Mag & b, Mag & q, Mag & r, bool remainder) const
{
    auto && a = *this;
    std::size_t n = b.size;
    std::size_t m = a.size - n;

    bool pass;
    ElemType quo, rem, t; // test quotient, remainder and a temp var
    ElemType high, low, divisor;
    Mag v = b;
    // D1 normalize
    int count = v.normalize();
    auto && u = a << count;
    // u.resize(m + n + 1);
    // D2 loop
    divisor = v[n - 1];
    for (std::size_t j = m + 1; j > 0; j--)
    {
        high = u[n + j - 1];
        low = u[n - 1 + j - 1];

        // D3
        assert(high <= divisor);
        if (high == divisor)
        {
            // quo == base
            // do the substraction and addtion
            assert(low < divisor);
            quo = elemMAX;
            rem = low + divisor;
            if (rem < low && rem > 0) // !(r < base)
                pass = true;
            else
                pass = false;
        }
        else
            std::tie(quo, rem) = divrem(high, low, divisor);
        while (!pass && (mullh(quo, v[n - 2]) > std::make_tuple(rem, u[n - 2 + j - 1])))
        {
            --quo;
            t = rem + divisor;
            if (t < rem && t > 0) // !(r < base)
                break;
            else
                rem = t;
        }
        // D4
        if (!u.subMul(v, quo, j - 1))
            // D6, no need for add back, handled in subMul
            ++quo;
        q[j - 1] = quo;
    }

    // get remainder
    if (remainder)
    {
        LargeType temp;
        temp = static_cast<LargeType>(u[0]) << (elemWIDTH - count);
        r[0] = static_cast<ElemType>(temp >> elemWIDTH);
        // remainder's size cannot be large than divisor
        for (std::size_t i = 1; i < b.size; i++)
        {
            if (u[i] == 0)
                break;
            temp = static_cast<LargeType>(u[i]) << (elemWIDTH - count);
            r[i] = static_cast<ElemType>(temp >> elemWIDTH);
            r[i - 1] |= static_cast<ElemType>(temp);
        }
    }
}

Mag Mag::operator/(const Mag & b) const
{
    auto && a = *this;
    if (b.size == 1)
        return a / b.back();
    Mag r(a.size - b.size + 1);
    a.divFull(b, r, r, false);
    if (r.back() == 0)
        r.size--;
    return r;
}

std::tuple<Mag, Mag> Mag::divrem(const Mag & b) const
{
    auto && a = *this;
    if (b.size == 1)
    {
        auto [quo, r] = a.divrem(b.back());
        if (r == 0)
            return {std::move(quo), Mag()};
        Mag rem(1);
        rem[0] = r;
        return {std::move(quo), std::move(rem)};
    }
    Mag q(a.size - b.size + 1);
    Mag r(b.size);
    a.divFull(b, q, r, true);
    if (q.back() == 0)
        q.size--;
    // remainder comes from u be substracted several times, so removeZero
    r.removeZero();
    return {std::move(q), std::move(r)};
}

std::tuple<Mag, BigInteger::ElemType> Mag::divrem(const ElemType n) const
{
    auto && v = *this;
    ElemType temp = v.back();
    // if first high elem isn't larger that n, r size is v.size - 1
    Mag r(v.size);
    if (temp >= n)
    {
        r.back() = temp / n;
        temp = temp % n;
    }
    for (std::size_t i = v.size - 1; i > 0; i--)
        std::tie(r[i - 1], temp) = divrem(temp, v[i - 1], n);
    return {std::move(r), temp};
}

Mag Mag::operator/(const ElemType n) const { return std::get<0>(this->divrem(n)); }

Mag Mag::operator&(const Mag & b) const
{
    auto && a = *this;
    swap_and_define;
    // 111 & 110101 = 000111 & 110101
    Mag r(minSize);
    std::size_t i = 0, t;
    std::generate_n(r.begin(), minSize, [&]() {
        t = i;
        i++;
        return v1[t] & v2[t];
    });
    return r;
}
Mag Mag::andnot(const Mag & v2) const
{
    auto && v1 = *this;
    Mag r(v1.size);
    if (v1.size < v2.size)
    {
        // 111 & ~110101 = 000111 & ~110101
        std::size_t i = 0, t;
        std::generate_n(r.begin(), v1.size, [&]() {
            t = i;
            i++;
            return v1[t] & ~v2[t];
        });
        return r;
    }
    else
    {
        // 110101 & ~111 = 110101 & 111000
        std::size_t i = 0, t;
        std::generate_n(r.begin(), v2.size, [&]() {
            t = i;
            i++;
            return v1[t] & ~v2[t];
        });
        for (; i < v1.size; i++)
            r[i] = v1[i];
        return r;
    }
}
Mag Mag::operator|(const Mag & b) const
{
    auto && a = *this;
    swap_and_define;
    Mag r(maxSize);
    std::size_t i = 0, t;
    auto count = v2.size;
    std::generate_n(r.begin(), count, [&]() {
        t = i;
        i++;
        return v1[t] | v2[t];
    });
    for (; i < maxSize; i++)
        r[i] = v1[i];
    return r;
}
Mag Mag::operator^(const Mag & b) const
{
    auto && a = *this;
    swap_and_define;
    Mag r(maxSize);
    std::size_t i = 0, t;
    auto count = v2.size;
    std::generate_n(r.begin(), count, [&]() {
        t = i;
        i++;
        return v1[t] ^ v2[t];
    });
    for (; i < maxSize; i++)
        r[i] = v1[i];
    return r;
}

Mag Mag::operator<<(const ElemType n) const
{
    auto && v = *this;
    LargeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    Mag r(v.size + count + 1);
    for (std::size_t i = 0; i < v.size; i++)
    {
        t = v[i];
        t <<= rem;
        r[count + i] |= t;
        r[count + i + 1] = t >> elemWIDTH;
    }
    if (r.back() == 0)
        r.size--;
    return r;
}
Mag Mag::operator>>(const ElemType n) const
{
    auto && v = *this;
    LargeType t;
    std::size_t count, rem;
    count = n / elemWIDTH;
    rem = n % elemWIDTH;
    assert(count < v.size);
    Mag r(v.size - count);
    t = static_cast<LargeType>(v[count]) << (elemWIDTH - rem);
    r[0] = static_cast<ElemType>(t >> elemWIDTH);
    for (std::size_t i = 1 + count; i < v.size; i++)
    {
        t = static_cast<LargeType>(v[i]) << (elemWIDTH - rem);
        r[i - count] = static_cast<ElemType>(t >> elemWIDTH);
        r[i - count - 1] |= static_cast<ElemType>(t);
    }
    if (r.back() == 0)
        r.size--;
    return r;
}

const std::array<double, 37> Mag::log2 = {0, 0, 1.0, 0.6309297535714574, 0.5, 0.43067655807339306,
    0.3868528072345416, 0.3562071871080222, 0.33333333333333337, 0.3154648767857287, 0.30102999566398114,
    0.2890648263178878, 0.2789429456511298, 0.27023815442731974, 0.26264953503719357, 0.2559580248098155, 0.25,
    0.244650542118226, 0.23981246656813146, 0.23540891336663824, 0.23137821315975918, 0.227670248696953,
    0.2242438242175754, 0.22106472945750374, 0.21810429198553155, 0.21533827903669653, 0.21274605355336315,
    0.21030991785715247, 0.20801459767650946, 0.20584683246043445, 0.20379504709050617, 0.20184908658209985,
    0.19999999999999998, 0.19823986317056053, 0.19656163223282258, 0.1949590218937863, 0.1934264036172708};
constexpr std::array<int, 37> Mag::calcDigits()
{
    std::array<int, 37> a{};
    // one binray digits can represenst log(i) of base i
    for (size_t i = 2; i < a.size(); i++)
        a[i] = std::numeric_limits<ElemType>::digits * log2[i];
    return a;
}
constexpr std::array<BigInteger::ElemType, 37> Mag::calcMax()
{
    std::array<ElemType, 37> a{};
    for (size_t i = 2; i < a.size(); i++)
    {
        ElemType v = 1;
        // calculate power
        for (int j = 0; j < digitsPerElem[i]; j++)
            v *= i;
        if (v != 0)
            a[i] = v;
        else
            a[i] = std::numeric_limits<ElemType>::max();
    }
    return a;
}
// order is important here
const std::array<int, 37> Mag::digitsPerElem = calcDigits();
const std::array<BigInteger::ElemType, 37> Mag::maxPerElem = calcMax();

/**
 * @brief helper Function to convert char to int
 *
 * @param ch
 * @param base
 * @throw BigIntegerException when ch is invalid
 */
int Mag::digit(char ch, int base)
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

BigInteger::ElemType Mag::convert(const char * str, int base, int length)
{
    ElemType num = 0;
    for (int i = 0; i < length; i++)
    {
        num *= base;
        num += digit(std::tolower(str[i]), base);
    }
    return num;
}

void Mag::addMul(const ElemType p, const ElemType n)
{
    auto && v = *this;
    LargeType temp;
    ElemType carry = 0;
    assert(v.size > 0);
    for (std::size_t i = 0; i < v.size; i++)
    {
        temp = static_cast<LargeType>(p) * v[i] + carry;
        carry = temp >> elemWIDTH;
        v[i] = static_cast<ElemType>(temp);
    }
    if (carry)
    {
        v.size++;
        v.back() = carry;
    }
    ElemType sl;
    sl = v[0] + n;
    carry = sl < v[0];
    v[0] = sl;
    for (std::size_t i = 1; i < v.size && carry != 0; i++)
    {
        sl = v[i] + carry;
        carry = sl == 0; // if overflow, only possibility is 0
        v[i] = sl;
    }
    if (carry)
    {
        v.size++;
        v.back() = carry;
    }
}

std::tuple<Mag, int> Mag::convert(const char * s, std::size_t len, int base)
{
    int sign = 1;
    int step = digitsPerElem[base];
    ElemType max = maxPerElem[base];
    char ch;

    const char * str = s;
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

    ch = *str;
    if (ch == 0 || std::isspace(ch))
        return {Mag(), 0};

    const char * start = str;
    // find first space
    while (ch != 0 && !std::isspace(ch))
    {
        str++;
        ch = *str;
    }
    std::size_t realLength = len - (start - s);
    if (ch != 0)
        realLength -= (str - start);

    auto slice = realLength / step;
    auto rem = realLength % step;
    Mag v(slice + (rem ? 1 : 0));
    v.size = 0;
    if (rem > 0)
    {
        v.size++;
        v.back() = convert(start, base, rem);
        start += rem;
    }
    else
    {
        v.size++;
        v.back() = convert(start, base, step);
        slice -= 1;
        start += step;
    }

    for (std::size_t i = 0; i < slice; i++)
    {
        v.addMul(max, convert(start, base, step));
        start += step;
    }
    return std::make_tuple(std::move(v), sign);
}

BigInteger::ElemType Mag::divremMutable(const ElemType n)
{
    auto && v = *this;
    ElemType temp = v.back();
    v.back() = temp / n;
    temp %= n;
    for (std::size_t i = v.size - 1; i > 0; i--)
        std::tie(v[i - 1], temp) = divrem(temp, v[i - 1], n);
    if (v.back() == 0)
        v.size--;
    return temp;
}

char Mag::toChar(ElemType e, int base)
{
    assert(e < base);
    if (e < 10)
        return '0' + e;
    else
        return 'a' + e - 10;
}

std::string Mag::toRawString(int base, bool upper) const
{
    if (base < 2 || base > 36)
        throw BigIntegerException("Invalid base");
    if (this->size == 0)
        return "0";
    std::ostringstream s;
    auto v = *this;
    ElemType rem;
    char ch;
    while (v.size != 0)
    {
        rem = v.divremMutable(base);
        ch = toChar(rem, base);
        if (upper)
            ch = toupper(ch);
        s << ch;
    }
    return s.str();
}