#ifndef __BIGINTEGER_HPP__
#define __BIGINTEGER_HPP__

#include <array>
#include <cmath>
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
        const static SignedElemType signedElemMax = std::numeric_limits<SignedElemType>::max();
        const static SignedElemType signedElemMin = std::numeric_limits<SignedElemType>::min();
        const static ElemType elemMAX = std::numeric_limits<ElemType>::max();
        const static ElemType elemWIDTH = std::numeric_limits<ElemType>::digits;

        struct Magnitude
        {
            using LargeType = __uint128_t;
            using TwoElemType = std::tuple<ElemType, ElemType>;
            Magnitude() = default;
            Magnitude(std::size_t);
            Magnitude(const Magnitude & b) : vec(b.copy()), size(b.size) {}
            Magnitude(Magnitude && mag) : vec(std::move(mag.vec)), size(mag.size) {}
            Magnitude & operator=(const Magnitude & v)
            {
                this->vec = v.copy();
                this->size = v.size;
                return *this;
            }
            Magnitude & operator=(Magnitude && v)
            {
                this->vec = std::move(v.vec);
                this->size = v.size;
                return *this;
            }
            ~Magnitude() = default;
            std::unique_ptr<ElemType[]> vec;
            std::size_t size = 0;

            // mimic vector's action, easier to change
            inline ElemType & operator[](std::size_t i) { return vec[i]; }
            inline const ElemType & operator[](std::size_t i) const { return vec[i]; }
            inline void resize(std::size_t newSize) { this->size = newSize; }
            inline ElemType & back() { return vec[size - 1]; }
            inline const ElemType & back() const { return vec[size - 1]; }
            inline ElemType & at(std::size_t i) { return vec[i]; }
            inline const ElemType & at(std::size_t i) const { return vec[i]; }
            inline ElemType * begin() { return vec.get(); }
            inline const ElemType * begin() const { return vec.get(); }

            const static std::array<double, 37> log2;
            constexpr static std::array<int, 37> calcDigits();
            constexpr static std::array<ElemType, 37> calcMax();
            const static std::array<int, 37> digitsPerElem;
            const static std::array<ElemType, 37> maxPerElem;

            Magnitude operator+(const ElemType) const;
            Magnitude operator+(const Magnitude &) const;
            Magnitude operator-(const ElemType) const;
            Magnitude operator-(const Magnitude &) const;
            Magnitude operator*(const Magnitude &) const;
            Magnitude longMul(const ElemType) const;
            Magnitude longMul(const Magnitude &) const;
            Magnitude operator/(const ElemType) const;
            std::tuple<Magnitude, ElemType> divrem(const ElemType) const;
            Magnitude operator/(const Magnitude &) const;
            std::tuple<Magnitude, Magnitude> divrem(const Magnitude &) const;
            void divFull(const Magnitude & v, Magnitude & q, Magnitude & r, bool rem) const;
            int subMul(const Magnitude & v, const ElemType q, const std::size_t index);
            void subMutable(const Magnitude & s, const std::size_t index);
            int normalize();
            Magnitude operator&(const Magnitude &) const;
            Magnitude andnot(const Magnitude &) const;
            Magnitude operator|(const Magnitude &) const;
            Magnitude operator^(const Magnitude &) const;
            Magnitude operator<<(const ElemType) const;
            Magnitude operator>>(const ElemType) const;
            void addMul(const ElemType, const ElemType);
            ElemType divremMutable(const ElemType);
            std::unique_ptr<ElemType[]> copy() const;
            std::string toRawString(int base, bool upper) const;

            static std::tuple<Magnitude, int> convert(const char *, std::size_t len, int base);
            inline static TwoElemType divrem(const ElemType h, const ElemType l, const ElemType divisor);
            inline static TwoElemType mullh(const ElemType, const ElemType);
            int compare(const Magnitude &) const;
            inline void removeZero();
            inline static int digit(char, int base);
            inline static char toChar(const ElemType, int base);
            inline static ElemType convert(const char *, int base, int length);
        };
        Magnitude magnitude;
        int sign = 0;

        inline static bool equalNegOne(const BigInteger &);
        BigInteger(Magnitude &&, int sign);

    public:
        BigInteger() = default;
        BigInteger(const SignedElemType);
        BigInteger(const int);
        explicit BigInteger(const std::string &);
        explicit BigInteger(const std::string &, int base);
        explicit BigInteger(const char *, std::size_t len, int base);
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
        void negate();
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