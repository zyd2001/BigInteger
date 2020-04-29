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
    private:
        std::shared_ptr<std::vector<elemType>> magnitude;
        int sign = 0;

        const static int digitsPerInt[35];
        const static elemType elemMAX = UINT64_MAX;
        const static elemType elemHALF = UINT64_MAX / 2 + 1;
        const static elemType elemWIDTH = sizeof(elemType);

        const static BigInteger zero;

        inline static int digit(char, int base);
        static std::shared_ptr<std::vector<elemType>> add(const std::vector<elemType>&, const elemType);
        static std::shared_ptr<std::vector<elemType>> add(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> sub(const std::vector<elemType>&, const elemType);
        static std::shared_ptr<std::vector<elemType>> sub(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> mul(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> longMul(const std::vector<elemType>&, const elemType);
        static std::shared_ptr<std::vector<elemType>> longMul(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> div(const std::vector<elemType>&, const elemType);
        static std::shared_ptr<std::vector<elemType>> div(const std::vector<elemType>&, const std::vector<elemType>&);
        static int subMul(std::vector<elemType> & u, const std::vector<elemType> & v, const elemType q);
        static void subMutable(std::vector<elemType> & u, const std::vector<elemType> & s);
        static int sl(std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> andFunc(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> andnot(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> orFunc(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> xorFunc(const std::vector<elemType>&, const std::vector<elemType>&);
        static std::shared_ptr<std::vector<elemType>> sl(const std::vector<elemType>&, const elemType);
        static std::shared_ptr<std::vector<elemType>> sr(const std::vector<elemType>&, const elemType);
        inline static std::tuple<elemType, elemType> divrem(const elemType h, const elemType l, const elemType divisor);
        inline static std::tuple<elemType, elemType> mullh(const elemType, const elemType);
        static int compare(const std::vector<elemType> &, const std::vector<elemType> &);
        static int convert(std::vector<elemType>&, const char *, int base);

    public:
        BigInteger() : magnitude(std::make_shared<std::vector<elemType>>()) {}
        BigInteger(const signedElemType);
        BigInteger(const std::string &);
        BigInteger(const std::string &, int base);
        BigInteger(const char *);
        BigInteger(const char *, std::size_t len, int base);
        BigInteger(const char *, int base);
        BigInteger(const std::vector<elemType>, int sign);
        BigInteger(const std::shared_ptr<std::vector<elemType>>, int sign);
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