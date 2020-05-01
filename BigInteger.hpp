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
        using elemType = uint64_t;
        using signedElemType = int64_t;
        using largeType = __uint128_t;
        using magVec = std::vector<elemType>;
        using magType = std::shared_ptr<magVec>;
        using twoElemType = std::tuple<elemType, elemType>;
    private:
        magType magnitude;
        int sign = 0;

        const static int digitsPerInt[35];
        const static elemType elemMAX = UINT64_MAX;
        const static elemType elemWIDTH = sizeof(elemType);

        const static BigInteger zero;

        inline static int digit(char, int base);
        static magType add(const magVec &, const elemType);
        static magType add(const magVec &, const magVec &);
        static magType sub(const magVec &, const elemType);
        static magType sub(const magVec &, const magVec &);
        static magType mul(const magVec &, const magVec &);
        static magType longMul(const magVec &, const elemType);
        static magType longMul(const magVec &, const magVec &);
        static magType div(const magVec &, const elemType);
        static magType div(const magVec &, const magVec &);
        static std::tuple<magType, magType> divrem(const magVec &, const magVec &);
        static void divFull(const magVec & u, const magVec & v, magVec & q, magVec & r, bool rem);
        static int subMul(magVec & u, const magVec & v, const elemType q, const std::size_t index);
        static void subMutable(magVec & u, const magVec & s, const std::size_t index);
        static int normalize(magVec &);
        static magType andFunc(const magVec &, const magVec &);
        static magType andnot(const magVec &, const magVec &);
        static magType orFunc(const magVec &, const magVec &);
        static magType xorFunc(const magVec &, const magVec &);
        static magType sl(const magVec &, const elemType);
        static magType sr(const magVec &, const elemType);
        inline static twoElemType divrem(const elemType h, const elemType l, const elemType divisor);
        inline static twoElemType mullh(const elemType, const elemType);
        static int compare(const magVec  &, const magVec  &);
        static int convert(magVec &, const char *, int base);

    public:
        BigInteger() : magnitude(std::make_shared<magVec >()) {}
        BigInteger(const signedElemType);
        BigInteger(const std::string &);
        BigInteger(const std::string &, int base);
        BigInteger(const char *);
        BigInteger(const char *, std::size_t len, int base);
        BigInteger(const char *, int base);
        BigInteger(const magVec , int sign);
        BigInteger(const magType, int sign);
        BigInteger(const BigInteger &) = default;
        BigInteger operator+(const BigInteger &) const;
        BigInteger operator-(const BigInteger &) const;
        BigInteger operator*(const BigInteger &) const;
        BigInteger operator/(const BigInteger &) const;
        BigInteger operator%(const BigInteger &) const;
        BigInteger operator-() const;
        operator bool() const;
        BigInteger operator<<(const elemType) const;
        BigInteger operator>>(const elemType) const;
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