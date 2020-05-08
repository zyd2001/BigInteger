#ifndef __BIGINTEGER_HPP__
#define __BIGINTEGER_HPP__

#include <array>
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

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
        using QuoRemType = std::tuple<BigInteger, BigInteger>;

    private:
        using LargeType = __uint128_t;
        using Vec = std::vector<ElemType>;
        using VecPtr = std::shared_ptr<Vec>;
        using TwoElemType = std::tuple<ElemType, ElemType>;
        VecPtr magnitude;
        int sign = 0;

        const static std::array<double, 37> log2;
        constexpr static std::array<int, 37> calcDigits();
        constexpr static std::array<ElemType, 37> calcMax();
        const static std::array<int, 37> digitsPerElem;
        const static std::array<ElemType, 37> maxPerElem;
        const static SignedElemType signedElemMax = std::numeric_limits<SignedElemType>::max();
        const static SignedElemType signedElemMin = std::numeric_limits<SignedElemType>::min();
        const static ElemType elemMAX = std::numeric_limits<ElemType>::max();
        const static ElemType elemWIDTH = std::numeric_limits<ElemType>::digits;

        const static BigInteger zero;
        const static VecPtr zeroPtr;

        static VecPtr add(const Vec &, const ElemType);
        static VecPtr add(const Vec &, const Vec &);
        static VecPtr sub(const Vec &, const ElemType);
        static VecPtr sub(const Vec &, const Vec &);
        static VecPtr mul(const Vec &, const Vec &);
        static VecPtr longMul(const Vec &, const ElemType);
        static VecPtr longMul(const Vec &, const Vec &);
        static VecPtr div(const Vec &, const ElemType);
        static std::tuple<VecPtr, ElemType> divrem(const Vec &, const ElemType);
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
        static int compare(const Vec &, const Vec &);
        inline static int digit(char, int base);
        inline static char toChar(const ElemType, int base);
        inline static ElemType convert(const char *, int base, int length);
        static int convert(Vec &, const char *, std::size_t len, int base);
        static void addMul(Vec &, const ElemType, const ElemType);
        static ElemType divremMutable(Vec &, const ElemType);
        inline static void removeZero(Vec &);
        inline static bool equalNegOne(const BigInteger &);

        BigInteger(const Vec &, int sign);
        BigInteger(const VecPtr &, int sign);

    public:
        BigInteger() : magnitude(zeroPtr) {}
        BigInteger(const SignedElemType);
        BigInteger(const int);
        explicit BigInteger(const std::string &);
        explicit BigInteger(const std::string &, int base);
        explicit BigInteger(const char *, std::size_t len, int base);
        BigInteger(const BigInteger &) = default;
        BigInteger operator+(const BigInteger &) const;
        BigInteger operator-(const BigInteger &) const;
        BigInteger operator*(const BigInteger &) const;
        BigInteger operator/(const BigInteger &) const;
        BigInteger operator%(const BigInteger &) const;
        QuoRemType div(const BigInteger &) const;
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
        std::string toString(int base, bool upper) const;

        friend std::ostream & operator<<(std::ostream &, const BigInteger &);
        friend std::istream & operator>>(std::istream &, BigInteger &);
        friend class BigIntegerTest;
    };

    std::ostream & operator<<(std::ostream &, const BigInteger &);
    std::istream & operator>>(std::istream &, BigInteger &);

    struct BigIntegerException : std::exception
    {
    private:
        std::string msg;

    public:
        BigIntegerException(const std::string & s) : msg(s) {}
        const char * what() const noexcept override { return msg.c_str(); }
    };

    namespace BigIntegerLiteral
    {
        BigInteger operator"" _BI(const char * s, std::size_t i);
    }
} // namespace zyd2001

#endif