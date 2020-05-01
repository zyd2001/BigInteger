#ifndef __BIGINTEGER_HPP__
#define __BIGINTEGER_HPP__

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <exception>
#include <tuple>

namespace zyd2001
{
    /**
     * @brief arbitrary-precision integers
     * 
     */
    struct BigInteger
    {
    public:
        using ElemType = uint64_t;
        using SignedElemType = int64_t;
        using LargeType = __uint128_t;
        using Vec = std::vector<ElemType>;
        using VecPtr = std::shared_ptr<Vec>;
        using TwoElemType = std::tuple<ElemType, ElemType>;
    private:
        VecPtr magnitude;
        int sign = 0;

        const static int digitsPerInt[35];
        const static ElemType elemMAX = UINT64_MAX;
        const static ElemType elemWIDTH = sizeof(ElemType);

        const static BigInteger zero;

        inline static int digit(char, int base);
        static VecPtr add(const Vec &, const ElemType);
        static VecPtr add(const Vec &, const Vec &);
        static VecPtr sub(const Vec &, const ElemType);
        static VecPtr sub(const Vec &, const Vec &);
        static VecPtr mul(const Vec &, const Vec &);
        static VecPtr longMul(const Vec &, const ElemType);
        static VecPtr longMul(const Vec &, const Vec &);
        static VecPtr div(const Vec &, const ElemType);
        static VecPtr div(const Vec &, const Vec &);
        static std::tuple<VecPtr, VecPtr> divrem(const Vec &, const Vec &);
        static void divFull(const Vec & u, const Vec & v, Vec & q, Vec & r, bool rem);
        static int subMul(Vec & u, const Vec & v, const ElemType q, const std::size_t index);
        static void subMutable(Vec & u, const Vec & s, const std::size_t index);
        static int normalize(Vec &);
        static VecPtr andFunc(const Vec &, const Vec &);
        static VecPtr andnot(const Vec &, const Vec &);
        static VecPtr orFunc(const Vec &, const Vec &);
        static VecPtr xorFunc(const Vec &, const Vec &);
        static VecPtr sl(const Vec &, const ElemType);
        static VecPtr sr(const Vec &, const ElemType);
        inline static TwoElemType divrem(const ElemType h, const ElemType l, const ElemType divisor);
        inline static TwoElemType mullh(const ElemType, const ElemType);
        static int compare(const Vec  &, const Vec  &);
        static int convert(Vec &, const char *, int base);

    public:
        BigInteger() : magnitude(std::make_shared<Vec >()) {}
        BigInteger(const SignedElemType);
        BigInteger(const std::string &);
        BigInteger(const std::string &, int base);
        BigInteger(const char *);
        BigInteger(const char *, std::size_t len, int base);
        BigInteger(const char *, int base);
        BigInteger(const Vec , int sign);
        BigInteger(const VecPtr, int sign);
        BigInteger(const BigInteger &) = default;
        BigInteger operator+(const BigInteger &) const;
        BigInteger operator-(const BigInteger &) const;
        BigInteger operator*(const BigInteger &) const;
        BigInteger operator/(const BigInteger &) const;
        BigInteger operator%(const BigInteger &) const;
        BigInteger operator-() const;
        explicit operator bool() const;
        BigInteger operator<<(const ElemType) const;
        BigInteger operator>>(const ElemType) const;
        BigInteger operator~() const;
        BigInteger operator&(const BigInteger &) const;
        BigInteger operator|(const BigInteger &) const;
        BigInteger operator^(const BigInteger &) const;
        bool operator>(const BigInteger &) const;
        bool operator>=(const BigInteger &) const;
        bool operator<(const BigInteger &) const;
        bool operator<=(const BigInteger &) const;
        bool operator==(const BigInteger &) const;
        bool operator!=(const BigInteger &) const;
        std::string toString() const;
        std::string toString(int base) const;

        friend std::ostream& operator<<(std::ostream&, const BigInteger &);
        friend std::istream& operator>>(std::istream&, BigInteger &);
    };

    std::ostream& operator<<(std::ostream&, const BigInteger &);
    std::istream& operator>>(std::istream&, BigInteger &);

    struct BigIntegerException : std::exception
    {
    public:
        std::string msg;
        BigIntegerException(const std::string & s) : msg(s) {}
    };
}


#endif