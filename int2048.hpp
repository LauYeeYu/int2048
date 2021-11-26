// int2048
// Lau YeeYu

#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER


/**
 * COMPRESSED IMPLEMENTATION: By using "std::vector<long long>" to store every
 * 9 digits (decimal-based).
 */

/**
 * Integer 1:
 * To implement a signed big int class, only supporting simple addition and
 * subtraction.
 * 實現一個有符號的大整數類，僅需支援簡單加減法。
 */

/**
 * Integer 2:
 * To implement a signed big int class, supporting addition, subtraction,
 * multiplication and division, and overriding related operator.
 * 實現一個有符號的大整數類，支援加減乘除，並重載相關運算符。
 */

#include <iostream>
#include <cstring>
#include <cstdio>
#include <utility>
#include <vector>
#include <complex>



namespace sjtu
{
    static constexpr long long Max = 1000000000; // i.e. 1000000000
    static constexpr int MaxDigit = 9; // i.e. 9
    static constexpr long long Power[9] = {1, 10, 100, 1000, 10000, 100000,
                                           1000000, 10000000, 100000000};

    /// To reduce the time of allocating memory for vector BigInt
    /// , but the side effect is larger default size of int2048;
    static constexpr int Reserve = 32;

    bool AbsCompareLess(const std::vector<long long>& A, int Start, int End,
                        const std::vector<long long>& B, int BLength);

    class int2048
    {
    private:
        /// reversed big int
        std::vector<long long> BigInt = std::vector<long long>(Reserve, 0);
        bool IsPositive = true;
        int Size = 1; /// The number of elements used in the vector

        /// Abs add, return |*this| = |*this| + |B|
        int2048& _simple_add(const int2048& B);

        /// Abs subtraction, return |*this| = |*this| - |B|
        /// (CAUTION: |*this| MUST be NO LESS THAN |B|)
        int2048& _simple_minus(const int2048& B);

        /// Reversed abs subtraction, return |*this| = |B| - |*this|
        /// (CAUTION: |B| MUST be NO LESS THAN |*this|)
        int2048& _reversed_simple_minus(const int2048& B);

        /// Resize the "BigInt" (For clear zero in the beginning of the int)
        int2048& _resize();

        std::vector<long long>& _single_multiply(std::vector<long long>&,
                                                 long long A) const;

    public:
        // Constructor
        int2048();
        int2048(long long);
        int2048(const std::string&);
        int2048(const int2048&);
        int2048(int2048&&) noexcept;


        /// Load a Big Int
        void read(const std::string&);

        /// Print the Big Int
        void print() const;

        /// Add another Big Int
        int2048& add(const int2048&);
        /// Return the Sum of Two int2048
        friend int2048 add(int2048 A, const int2048& B);

        /// Minus another Big Int
        int2048& minus(const int2048&);
        /// Return the Subtraction of Two int2048
        friend int2048 minus(int2048 A, const int2048& B);

        int2048& operator=(const int2048&);

        int2048& operator+=(const int2048&);
        friend int2048 operator+(int2048, const int2048&);

        int2048& operator-=(const int2048&);
        friend int2048 operator-(int2048, const int2048&);

        int2048& operator*=(int2048);
        friend int2048 operator*(int2048, const int2048&);

        int2048& operator/=(int2048);
        friend int2048 operator/(int2048, const int2048&);

        friend std::istream& operator>>(std::istream &, int2048&);
        friend std::ostream& operator<<(std::ostream &, const int2048&);

        friend int AbsCompare(const int2048&, const int2048&);

        inline friend bool operator==(const int2048&, const int2048&);
        inline friend bool operator!=(const int2048&, const int2048&);
        inline friend bool operator<(const int2048&, const int2048&);
        inline friend bool operator>(const int2048&, const int2048&);
        inline friend bool operator<=(const int2048&, const int2048&);
        inline friend bool operator>=(const int2048&, const int2048&);
    };

    int2048::int2048()
    {
        BigInt.push_back(0);
    }

    int2048::int2048(long long x)
    {
        if (x == 0) {
            BigInt.push_back(0);
        } else {
            if (x < 0) {
                IsPositive = false;
                x = -x;
            }
            int i = 0;
            while (x != 0) {
                BigInt.resize(3, 0);
                BigInt[i] = x % Max;
                x = x / Max;
                ++i;
            }
            Size = i;
        }
    }

    int2048::int2048(const std::string& String)
    {
        if (String[0] == '-') {
            if (String[1] == '0') {
                BigInt.push_back(0);
            } else {
                IsPositive = false;
                Size = (String.length() - 2) / MaxDigit + 1;
                BigInt.resize(Size, 0);
                int i = String.length() - 1;
                int Count = 0;
                 while ( i >= MaxDigit) {
                     for (int j = 0; j < MaxDigit; ++j) {
                         BigInt[Count] += (String[i - j] - 48) * Power[j];
                     }
                     i -= MaxDigit;
                     ++Count;
                 }
                 for (int j = 0; i > 0; ++j) {
                     BigInt[Count] += (String[i] - 48) * Power[j];
                     --i;
                 }
            }
        } else {
            Size = (String.length() - 1) / MaxDigit + 1;
            BigInt.resize(Size, 0);
            int i = String.length() - 1;
            int Count = 0;
            while ( i >= MaxDigit - 1) {
                for (int j = 0; j < MaxDigit; ++j) {
                    BigInt[Count] += (String[i - j] - 48) * Power[j];
                }
                i -= MaxDigit;
                ++Count;
            }
            for (int j = 0; i >= 0; ++j) {
                BigInt[Count] += (String[i] - 48) * Power[j];
                --i;
            }
        }
    }

    int2048::int2048(const int2048&) = default;

    int2048::int2048(int2048&&) noexcept = default;

    void int2048::read(const std::string& String)
    {
        BigInt.clear();
        if (String[0] == '-') {
            if (String[1] == '0') {
                BigInt.push_back(0);
                IsPositive = true;
                Size = 1;
            } else {
                IsPositive = false;
                Size = (String.length() - 2) / MaxDigit + 1;
                BigInt.resize(Size, 0);
                int i = String.length() - 1;
                int Count = 0;
                while ( i >= MaxDigit) {
                    for (int j = 0; j < MaxDigit; ++j) {
                        BigInt[Count] += (String[i - j] - 48) * Power[j];
                    }
                    i -= MaxDigit;
                    ++Count;
                }
                for (int j = 0; i > 0; ++j) {
                    BigInt[Count] += (String[i] - 48) * Power[j];
                    --i;
                }
            }
        } else {
            IsPositive = true;
            Size = (String.length() - 1) / MaxDigit + 1;
            BigInt.resize(Size, 0);
            int i = String.length() - 1;
            int Count = 0;
            while ( i >= MaxDigit - 1) {
                for (int j = 0; j < MaxDigit; ++j) {
                    BigInt[Count] += (String[i - j] - 48) * Power[j];
                }
                i -= MaxDigit;
                ++Count;
            }
            for (int j = 0; i >= 0; ++j) {
                BigInt[Count] += (String[i] - 48) * Power[j];
                --i;
            }
        }
    }

    void int2048::print() const
    {
        std::cout << (*this);
    }

    int2048 &int2048::_simple_add(const int2048& B)
    {
        if (Size > B.Size) {
            BigInt.resize(Size + 1, 0);
            bool Carry = false;
            for (int i = 0; i < B.Size; ++i) {
                BigInt[i] = BigInt[i] + B.BigInt[i] + Carry;
                if (BigInt[i] >= Max) {
                    BigInt[i] -= Max;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Size; i < Size; ++i) {
                BigInt[i] = BigInt[i] + Carry;
                if (BigInt[i] >= Max) {
                    BigInt[i] -= Max;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                BigInt[Size] = 1;
                ++Size;
            }
        } else {
            Size = B.Size;
            BigInt.resize(Size + 1, 0);
            bool Carry = false;
            for (int i = 0; i < Size; ++i) {
                BigInt[i] = BigInt[i] + B.BigInt[i] + Carry;
                if (BigInt[i] >= Max) {
                    BigInt[i] -= Max;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                BigInt[Size] = 1;
                ++Size;
            }
        }
        return (*this);
    }

    int2048& int2048::_simple_minus(const int2048& B)
    {
        bool Carry = false;
        for (int i = 0; i < B.Size; ++i) {
            BigInt[i] = BigInt[i] - B.BigInt[i] - Carry;
            if (BigInt[i] < 0) {
                BigInt[i] += Max;
                Carry = true;
                continue;
            }
            Carry = false;
        }
        for (int i = B.Size; i < Size; ++i) {
            BigInt[i] = BigInt[i] - Carry;
            if (BigInt[i] < 0) {
                BigInt[i] += Max;
                Carry = true;
                continue;
            }
            Carry = false;
        }
        this->_resize();
        return (*this);
    }

    int2048& int2048::_reversed_simple_minus(const int2048& B)
    {
        bool Carry = false;
        BigInt.resize(B.Size, 0);
        for (int i = 0; i < B.Size; ++i) {
            BigInt[i] = B.BigInt[i] - BigInt[i] - Carry;
            if (BigInt[i] < 0) {
                BigInt[i] += Max;
                Carry = true;
                continue;
            }
            Carry = false;
        }
        Size = B.Size;
        this->_resize();
        return (*this);
    }

    int2048& int2048::_resize()
    {
        while (BigInt[Size - 1] == 0 && Size > 1) {
            --Size;
        }
        return (*this);
    }

    int2048 &int2048::add(const int2048& B)
    {
        return (*this += B);
    }

    int2048 add(int2048 A, const int2048 &B)
    {
        return (A += B);
    }

    int2048 &int2048::minus(const int2048& B)
    {
        return (*this -= B);
    }

    int2048 minus(int2048 A, const int2048& B)
    {
        return (A -= B);
    }

    int2048& int2048::operator=(const int2048&) = default;

    int2048& int2048::operator+=(const int2048& B)
    {
        if ((IsPositive && B.IsPositive) || (!IsPositive && !B.IsPositive)) {
            this->_simple_add(B);
        } else {
            if (AbsCompare(*this, B) == 1) {
                this->_simple_minus(B);
            } else {
                this->_reversed_simple_minus(B);
                IsPositive = !IsPositive;
            }
        }
        return (*this);
    }

    int2048 operator+(int2048 A, const int2048& B)
    {
        return (A += B);
    }

    int2048 &int2048::operator-=(const int2048& B)
    {
        if ((!IsPositive && B.IsPositive) || (IsPositive && !B.IsPositive)) {
            this->_simple_add(B);
        } else {
            if (AbsCompare(*this, B) == 1) {
                this->_simple_minus(B);
            } else {
                this->_reversed_simple_minus(B);
                IsPositive = !IsPositive;
            }
        }
        return (*this);
    }

    int2048 operator-(int2048 A, const int2048& B)
    {
        return (A -= B);
    }

    int2048 &int2048::operator*=(int2048 B)
    {
        // For the IsPositive Flag
        if ((!IsPositive && B.IsPositive) || (IsPositive && !B.IsPositive)) {
            IsPositive = false;
        } else {
            IsPositive = true;
        }

        // For the Unsigned Multiplication
        if (B == 0 || (*this) == 0) {
            BigInt.clear();
            BigInt.push_back(0);
            Size = 1;
            IsPositive = true;
            return (*this);
        }
        std::vector<long long> A_Origin = BigInt;
        BigInt.resize(Size + B.Size, 0);
        long long Carry = 0;
        long long Temp;
        for (int A_Digit = 0; A_Digit < Size; ++A_Digit) {
            Temp = A_Origin[A_Digit] * B.BigInt[0] + Carry;
            Carry = Temp / Max;
            BigInt[A_Digit] = Temp % Max;
        }
        if (Carry != 0) {
            BigInt[Size] = Carry;
        }
        BigInt[Size] = Carry;
        for (int B_Digit = 1; B_Digit < B.Size; ++B_Digit) {
            Carry = 0;
            for (int A_Digit = 0; A_Digit < Size; ++A_Digit) {
                Temp = A_Origin[A_Digit] * B.BigInt[B_Digit] + Carry
                     + BigInt[A_Digit + B_Digit];
                Carry = Temp / Max;
                BigInt[A_Digit + B_Digit] = Temp % Max;
            }
            BigInt[Size + B_Digit] = Carry;
        }
        if (Carry != 0) {
            Size = Size + B.Size;
        } else {
            Size = Size + B.Size - 1;
        }
        return (*this);
    }

    int2048 operator*(int2048 A, const int2048& B)
    {
        // For the IsPositive Flag
        if ((!A.IsPositive && B.IsPositive) || (A.IsPositive && !B.IsPositive)) {
            A.IsPositive = false;
        } else {
            A.IsPositive = true;
        }

        // For the Unsigned Multiplication
        if (A == 0 || B == 0) {
            A.BigInt.clear();
            A.BigInt.push_back(0);
            A.Size = 1;
            A.IsPositive = true;
            return A;
        }
        std::vector<long long> A_Origin = A.BigInt;
        A.BigInt.resize(A.Size + B.Size, 0);
        long long Carry = 0;
        long long Temp;
        for (int A_Digit = 0; A_Digit < A.Size; ++A_Digit) {
            Temp = A_Origin[A_Digit] * B.BigInt[0] + Carry;
            Carry = Temp / Max;
            A.BigInt[A_Digit] = Temp % Max;
        }
        A.BigInt[A.Size] = Carry;
        for (int B_Digit = 1; B_Digit < B.Size; ++B_Digit) {
            Carry = 0;
            for (int A_Digit = 0; A_Digit < A.Size; ++A_Digit) {
                Temp = A_Origin[A_Digit] * B.BigInt[B_Digit] + Carry
                     + A.BigInt[A_Digit + B_Digit];
                Carry = Temp / Max;
                A.BigInt[A_Digit + B_Digit] = Temp % Max;
            }
            A.BigInt[A.Size + B_Digit] = Carry;
        }
        if (Carry != 0) {
            A.Size = A.Size + B.Size;
        } else {
            A.Size = A.Size + B.Size - 1;
        }
        return A;
    }

    int2048& int2048::operator/=(int2048 B)
    {
        // For the IsPositive Flag
        if ((!IsPositive && B.IsPositive) || (IsPositive && !B.IsPositive)) {
            IsPositive = false;
        } else {
            IsPositive = true;
        }

        // For the Unsigned Division
        if (Size < B.Size) {
            BigInt.clear();
            BigInt.push_back(0);
            Size = 1;
            IsPositive = true;
            return (*this);
        }
        std::vector<long long> TempA = BigInt;
        BigInt.clear();
        BigInt.resize(Size - B.Size + 1, 0);
        int ASize = Size;
        std::vector<long long> Temp = std::vector<long long>(B.Size + 1);
        Temp.resize(B.Size + 1, 0);
        for (int i = Size - B.Size; i >= 0; --i) {
            long long L = 0;
            long long R = Max - 1;

            // R * B
            int TempSize;
            Temp = B._single_multiply(Temp, R);
            if (Temp[B.Size] == 0) TempSize = B.Size;
            else TempSize = B.Size + 1;
            if (!AbsCompareLess(TempA, i, ASize, Temp, TempSize)) {
                bool Carry = false;
                for (int j = 0; j < TempSize; ++j) {
                    TempA[i + j] = TempA[i + j] - Temp[j] - Carry;
                    if (TempA[i + j] < 0) {
                        Carry = true;
                        TempA[i + j] += Max;
                        continue;
                    }
                    Carry = false;
                }
                if (Carry) --TempA[ASize - 1];
                BigInt[i] += R;
                while (TempA[ASize - 1] == 0 && ASize > 1) {
                    --ASize;
                }
                continue;
            }
            // Binary Search
            while (R - L > 1) {
                Temp = B._single_multiply(Temp, (L + R) / 2);
                if (Temp[B.Size] == 0) TempSize = B.Size;
                else TempSize = B.Size + 1;
                if (!AbsCompareLess(TempA, i, ASize, Temp, TempSize)) {
                    L = (L + R) / 2;
                } else {
                    R = (L + R) / 2;
                }
            }
            Temp = B._single_multiply(Temp, L);
            if (Temp[B.Size] == 0) TempSize = B.Size;
            else TempSize = B.Size + 1;
            bool Carry = false;
            for (int j = 0; j < TempSize; ++j) {
                TempA[i + j] = TempA[i + j] - Temp[j] - Carry;
                if (TempA[i + j] < 0) {
                    Carry = true;
                    TempA[i + j] += Max;
                    continue;
                }
                Carry = false;
            }
            if (Carry) --TempA[ASize - 1];
            BigInt[i] += L;
            while (TempA[ASize - 1] == 0 && ASize > 1) {
                --ASize;
            }
        }
        Size = Size - B.Size + 1;
        _resize();
        return (*this);
    }

    int2048 operator/(int2048 A, const int2048& B)
    {
        // For the IsPositive Flag
        if ((!A.IsPositive && B.IsPositive) || (A.IsPositive && !B.IsPositive)) {
            A.IsPositive = false;
        } else {
            A.IsPositive = true;
        }

        // For the Unsigned Division
        if (A.Size < B.Size) {
            A.BigInt.clear();
            A.BigInt.push_back(0);
            A.Size = 1;
            A.IsPositive = true;
            return A;
        }
        std::vector<long long> TempA = A.BigInt;
        A.BigInt.clear();
        A.BigInt.resize(A.Size - B.Size + 1, 0);
        int ASize = A.Size;
        std::vector<long long> Temp = std::vector<long long>(B.Size + 1);
        Temp.resize(B.Size + 1, 0);
        for (int i = A.Size - B.Size; i >= 0; --i) {
            long long L = 0;
            long long R = Max - 1;

            // R * B
            int TempSize;
            Temp = B._single_multiply(Temp, R);
            if (Temp[B.Size] == 0) TempSize = B.Size;
            else TempSize = B.Size + 1;
            if (!AbsCompareLess(TempA, i, ASize, Temp, TempSize)) {
                bool Carry = false;
                for (int j = 0; j < TempSize; ++j) {
                    TempA[i + j] = TempA[i + j] - Temp[j] - Carry;
                    if (TempA[i + j] < 0) {
                        Carry = true;
                        TempA[i + j] += Max;
                        continue;
                    }
                    Carry = false;
                }
                if (Carry) --TempA[ASize - 1];
                A.BigInt[i] += R;
                while (TempA[ASize - 1] == 0 && ASize > 1) {
                    --ASize;
                }
                continue;
            }
            // Binary Search
            while (R - L > 1) {
                Temp = B._single_multiply(Temp, (L + R) / 2);
                if (Temp[B.Size] == 0) TempSize = B.Size;
                else TempSize = B.Size + 1;
                if (!AbsCompareLess(TempA, i, ASize, Temp, TempSize)) {
                    L = (L + R) / 2;
                } else {
                    R = (L + R) / 2;
                }
            }
            Temp = B._single_multiply(Temp, L);
            if (Temp[B.Size] == 0) TempSize = B.Size;
            else TempSize = B.Size + 1;
            bool Carry = false;
            for (int j = 0; j < TempSize; ++j) {
                TempA[i + j] = TempA[i + j] - Temp[j] - Carry;
                if (TempA[i + j] < 0) {
                    Carry = true;
                    TempA[i + j] += Max;
                    continue;
                }
                Carry = false;
            }
            if (Carry) --TempA[ASize - 1];
            A.BigInt[i] += L;
            while (TempA[ASize - 1] == 0 && ASize > 1) {
                --ASize;
            }
        }
        A.Size = A.Size - B.Size + 1;
        A._resize();
        return A;
    }

    std::istream& operator>>(std::istream& InStream, int2048& Object)
    {
        std::string Temp;
        InStream >> Temp;
        Object.read(Temp);
        return InStream;
    }

    std::ostream& operator<<(std::ostream& OutStream, const int2048& Object)
    {
        if (!Object.IsPositive) OutStream << "-";
        OutStream << Object.BigInt[Object.Size - 1];
        for (int i = Object.Size - 2; i >= 0; --i) {
            long long Temp = Object.BigInt[i];
            for (int j = MaxDigit - 1; j >= 0; --j) {
                OutStream << (Temp / Power[j]);
                Temp = Temp % Power[j];
            }
        }
        return OutStream;
    }

    bool operator==(const int2048& A, const int2048& B)
    {
        if ((A.IsPositive && B.IsPositive) || (!A.IsPositive && !B.IsPositive)) {
            if (AbsCompare(A, B) == 0) return true;
        }
        return false;
    }

    bool operator!=(const int2048& A, const int2048& B)
    {
        return (!(A == B));
    }

    bool operator<(const int2048& A, const int2048& B)
    {
        if (A.IsPositive && !B.IsPositive) return false;
        if (!A.IsPositive && B.IsPositive) return true;
        if (A.IsPositive) {
            if (AbsCompare(A, B) == -1) return true;
            else return false;
        } else {
            if (AbsCompare(A, B) == 1) return true;
            else return false;
        }
    }

    bool operator>(const int2048& A, const int2048& B)
    {
        if (A.IsPositive && !B.IsPositive) return true;
        if (!A.IsPositive && B.IsPositive) return false;
        if (A.IsPositive) {
            if (AbsCompare(A, B) == 1) return true;
            else return false;
        } else {
            if (AbsCompare(A, B) == -1) return true;
            else return false;
        }
    }

    bool operator<=(const int2048& A, const int2048& B)
    {
        if (A.IsPositive && !B.IsPositive) return false;
        if (!A.IsPositive && B.IsPositive) return true;
        if (A.IsPositive) {
            if (AbsCompare(A, B) == 1) return false;
            else return true;
        } else {
            if (AbsCompare(A, B) == -1) return false;
            else return true;
        }
    }

    bool operator>=(const int2048& A, const int2048& B)
    {
        if (A.IsPositive && !B.IsPositive) return true;
        if (!A.IsPositive && B.IsPositive) return false;
        if (A.IsPositive) {
            if (AbsCompare(A, B) == -1) return false;
            else return true;
        } else {
            if (AbsCompare(A, B) == 1) return false;
            else return true;
        }
    }

    /**
     * @brief To Compare the ABS of A and B
     * @param A
     * @param B
     * @return If |A| = |B|, return 0.
     * If |A| > |B|, return 1.
     * If |A| < |B|, return -1.
     */
    int AbsCompare(const int2048& A, const int2048& B)
    {
        if (A.Size != B.Size) {
            if (A.Size > B.Size) return 1;
            else return -1;
        }
        for (int i = A.Size - 1; i >= 0; --i) {
            if (A.BigInt[i] != B.BigInt[i]) {
                if (A.BigInt[i] > B.BigInt[i]) return 1;
                else return -1;
            }
        }
        return 0;
    }

    /**
     * @brief To Multiply a "long long int" and a "BigInt"
     * @param Object
     * @param A the long long int
     * @return the answer vector
     */
    std::vector<long long>& int2048::_single_multiply(std::vector<long long>& Answer,
                                                      long long A) const
    {
        long long Carry = 0;
        for (int i = 0; i < Size; ++i) {
            Answer[i] = A * BigInt[i] + Carry;
            Carry = Answer[i] / Max;
            Answer[i] = Answer[i] % Max;
        }
        Answer[Size] = Carry;
        return Answer;
    }

    bool AbsCompareLess(const std::vector<long long>& A, int Start, int End,
                        const std::vector<long long>& B, int BLength)
    {
        if ((End - Start) != BLength) return ((End - Start) < BLength);
        for (int i = BLength - 1; i >= 0; --i) {
            if (A[Start + i] != B[i]) return (A[Start + i] < B[i]);
        }
        return false;
    }
}

#endif