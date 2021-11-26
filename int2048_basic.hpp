// int2048
// Lau YeeYu

#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

/**
 * Implementation: By using "std::string" to store every digit (decimal-based).
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
    bool AbsCompareLess(const std::string& A, int Start, int End,
                        const std::string& B, int BLength);

    class int2048
    {
    private:
        std::string BigInt; // reversed big int
        bool IsPositive = true;
        int Length = 1;

    public:
        // Constructor
        int2048();
        int2048(long long);
        int2048(const std::string&);
        int2048(const int2048&);
        int2048(int2048&&) noexcept;

        // =================================== Integer1 ===================================

        /// Load a Big Int
        void read(const std::string &s);

        /// Print the Big Int
        void print() const;

        /// Add another Big Int
        int2048& add(const int2048&);
        /// Return the Sum of Two int2048
        friend int2048 add(int2048 A, const int2048& B);

        /// Minus another Big Int
        int2048 &minus(const int2048&);
        /// Return the Subtraction of Two int2048
        friend int2048 minus(int2048, const int2048 &);

        // =================================== Integer2 ===================================

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

        inline friend bool operator==(const int2048&, const int2048&);
        inline friend bool operator!=(const int2048&, const int2048&);
        inline friend bool operator<(const int2048&, const int2048&);
        inline friend bool operator>(const int2048&, const int2048&);
        inline friend bool operator<=(const int2048&, const int2048&);
        inline friend bool operator>=(const int2048&, const int2048&);
    };
}

sjtu::int2048::int2048()
{
    BigInt = "0";
}

sjtu::int2048::int2048(long long x)
{
    if (x != 0) {
        int i = 0;
        if (x < 0) {
            IsPositive = false;
            x = -x;
        }
        while (x != 0) {
            BigInt.push_back(char(x % 10 + 48));
            x = x / 10;
            ++i;
        }
        Length = i;
    } else {
        BigInt = "0";
    }
}

sjtu::int2048::int2048(const std::string& s)
{
    if (s[0] == '-') {
        if (s[1] != '0') {
            IsPositive = false;
            Length = s.length() - 1;
            BigInt.resize(Length, '0');
            for (int i = 0; i < Length; ++i) {
                BigInt[i] = s[Length - i];
            }
        } else {
            BigInt = "0";
        }
    } else {
        Length = s.length();
        BigInt.resize(Length, '0');
        for (int i = 0; i < Length; ++i) {
            BigInt[i] = s[Length - i - 1];
        }
        while (BigInt[Length - 1] == '0' && Length > 1) {
            --Length;
        }
    }
}

sjtu::int2048::int2048(const int2048& src) = default;

sjtu::int2048::int2048(int2048&& src) noexcept = default;

void sjtu::int2048::read(const std::string &s)
{
    BigInt.clear();
    if (s[0] == '-') {
        if (s[1] != '0') {
            IsPositive = false;
            Length = s.length() - 1;
            BigInt.resize(Length, '0');
            for (int i = 0; i < Length; ++i) {
                BigInt[i] = s[Length - i];
            }
        } else {
            BigInt = "0";
            IsPositive = true;
            Length = 1;
        }
    } else {
        IsPositive = true;
        Length = s.length();
        BigInt.resize(Length, '0');
        for (int i = 0; i < Length; ++i) {
            BigInt[i] = s[Length - i - 1];
        }
        while (BigInt[Length - 1] == '0' && Length > 1) {
            --Length;
        }
    }
}

void sjtu::int2048::print() const
{
    if (!IsPositive) std::cout << "-";
    for (int i = Length - 1; i >= 0; --i) {
        std::cout << BigInt[i];
    }
}

sjtu::int2048& sjtu::int2048::add(const sjtu::int2048& B)
{
    if ((IsPositive && B.IsPositive) || (!IsPositive && !B.IsPositive)) {
        if (Length > B.Length) {
            BigInt.resize(Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = BigInt[i] + B.BigInt[i] + Carry - 48;
                if (BigInt[i] > 57) {
                    BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < Length; ++i) {
                BigInt[i] = BigInt[i] + Carry;
                if (BigInt[i] > 57) {
                    BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                BigInt[Length] = '1';
                ++Length;
            }
        } else {
            Length = B.Length;
            BigInt.resize(Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < Length; ++i) {
                BigInt[i] = BigInt[i] + B.BigInt[i] + Carry - 48;
                if (BigInt[i] > 57) {
                    BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                BigInt[Length] = '1';
                ++Length;
            }
        }
        return (*this);
    }
    if (B.IsPositive) {
        IsPositive = true;
        if ((*this) < B) {
            bool Carry = false;
            BigInt.resize(B.Length, '0');
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = B.BigInt[i] - BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            Length = B.Length;
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        } else {
            IsPositive = false;
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = BigInt[i] - B.BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < Length; ++i) {
                BigInt[i] = BigInt[i] - Carry;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        }
    } else {
        IsPositive = false;
        if ((*this) < B) {
            IsPositive = true;
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = BigInt[i] - B.BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < Length; ++i) {
                BigInt[i] = BigInt[i] - Carry;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        } else {
            bool Carry = false;
            BigInt.resize(B.Length, '0');
            for(int i = 0; i < B.Length; ++i) {
                BigInt[i] = B.BigInt[i] - BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            Length = B.Length;
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        }
    }
    return (*this);
}

sjtu::int2048& sjtu::int2048::minus(const sjtu::int2048& B)
{
    if ((!IsPositive && B.IsPositive) || (IsPositive && !B.IsPositive)) {
        if (Length > B.Length) {
            BigInt.resize(Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = BigInt[i] + B.BigInt[i] + Carry - 48;
                if (BigInt[i] > 57) {
                    BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < Length; ++i) {
                BigInt[i] = BigInt[i] + Carry;
                if (BigInt[i] > 57) {
                    BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                BigInt[Length] = '1';
                ++Length;
            }
        } else {
            Length = B.Length;
            BigInt.resize(Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < Length; ++i) {
                BigInt[i] = BigInt[i] + B.BigInt[i] + Carry - 48;
                if (BigInt[i] > 57) {
                    BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                BigInt[Length] = '1';
                ++Length;
            }
        }
        return (*this);
    }
    if (B.IsPositive) {
        if ((*this) >= B) {
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = BigInt[i] - B.BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < Length; ++i) {
                BigInt[i] = BigInt[i] - Carry;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        } else {
            IsPositive = false;
            bool Carry = false;
            BigInt.resize(B.Length, '0');
            for(int i = 0; i < B.Length; ++i) {
                BigInt[i] = B.BigInt[i] - BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            Length = B.Length;
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        }
    } else {
        if ((*this) >= B) {
            IsPositive = true;
            bool Carry = false;
            BigInt.resize(B.Length, '0');
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = B.BigInt[i] - BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            Length = B.Length;
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        } else {
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                BigInt[i] = BigInt[i] - B.BigInt[i] - Carry + 48;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < Length; ++i) {
                BigInt[i] = BigInt[i] - Carry;
                if (BigInt[i] < 48) {
                    BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        }
    }
    return (*this);
}

sjtu::int2048 sjtu::add(sjtu::int2048 A, const sjtu::int2048& B)
{
    if ((A.IsPositive && B.IsPositive) || (!A.IsPositive && !B.IsPositive)) {
        if (A.Length > B.Length) {
            A.BigInt.resize(A.Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] + B.BigInt[i] + Carry - 48;
                if (A.BigInt[i] > 57) {
                    A.BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] + Carry;
                if (A.BigInt[i] > 57) {
                    A.BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                A.BigInt[A.Length] = '1';
                ++A.Length;
            }
        } else {
            A.Length = B.Length;
            A.BigInt.resize(A.Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] + B.BigInt[i] + Carry - 48;
                if (A.BigInt[i] > 57) {
                    A.BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                A.BigInt[A.Length] = '1';
                ++A.Length;
            }
        }
        return A;
    }
    if (B.IsPositive) {
        A.IsPositive = true;
        if (A <= B) {
            bool Carry = false;
            A.BigInt.resize(B.Length, '0');
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = B.BigInt[i] - A.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            A.Length = B.Length;
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        } else {
            A.IsPositive = false;
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - B.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - Carry;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        }
    } else {
        A.IsPositive = false;
        if (A <= B) {
            A.IsPositive = true;
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - B.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - Carry;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        } else {
            bool Carry = false;
            A.BigInt.resize(B.Length, '0');
            for(int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = B.BigInt[i] - A.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            A.Length = B.Length;
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        }
    }
    return A;
}

sjtu::int2048 sjtu::minus(sjtu::int2048 A, const sjtu::int2048& B)
{
    if ((!A.IsPositive && B.IsPositive) || (A.IsPositive && !B.IsPositive)) {
        if (A.Length > B.Length) {
            A.BigInt.resize(A.Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] + B.BigInt[i] + Carry - 48;
                if (A.BigInt[i] > 57) {
                    A.BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] + Carry;
                if (A.BigInt[i] > 57) {
                    A.BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                A.BigInt[A.Length] = '1';
                ++A.Length;
            }
        } else {
            A.Length = B.Length;
            A.BigInt.resize(A.Length + 1, '0');
            bool Carry = false;
            for (int i = 0; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] + B.BigInt[i] + Carry - 48;
                if (A.BigInt[i] > 57) {
                    A.BigInt[i] -= 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            if (Carry) { // The case when the biggest digit has a carry
                A.BigInt[A.Length] = '1';
                ++A.Length;
            }
        }
        return A;
    }
    if (B.IsPositive) {
        if (A >= B) {
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - B.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - Carry;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        } else {
            A.IsPositive = false;
            bool Carry = false;
            A.BigInt.resize(B.Length, '0');
            for(int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = B.BigInt[i] - A.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            A.Length = B.Length;
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        }
    } else {
        if (A >= B) {
            A.IsPositive = true;
            bool Carry = false;
            A.BigInt.resize(B.Length, '0');
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = B.BigInt[i] - A.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            A.Length = B.Length;
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        } else {
            bool Carry = false;
            for (int i = 0; i < B.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - B.BigInt[i] - Carry + 48;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            for (int i = B.Length; i < A.Length; ++i) {
                A.BigInt[i] = A.BigInt[i] - Carry;
                if (A.BigInt[i] < 48) {
                    A.BigInt[i] += 10;
                    Carry = true;
                    continue;
                }
                Carry = false;
            }
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        }
    }
    return A;
}

sjtu::int2048& sjtu::int2048::operator=(const sjtu::int2048& Origin) = default;

sjtu::int2048& sjtu::int2048::operator+=(const sjtu::int2048& B)
{
    return this->add(B);
}

sjtu::int2048 sjtu::operator+(sjtu::int2048 A, const sjtu::int2048& B)
{
    return add(std::move(A), B);
}

sjtu::int2048& sjtu::int2048::operator-=(const sjtu::int2048& B)
{
    return this->minus(B);
}

sjtu::int2048 sjtu::operator-(sjtu::int2048 A, const sjtu::int2048& B)
{
    return minus(std::move(A), B);
}

sjtu::int2048& sjtu::int2048::operator*=(sjtu::int2048 B)
{
    // For the IsPositive Flag
    if ((!IsPositive && B.IsPositive) || (IsPositive && !B.IsPositive)) {
        IsPositive = false;
    } else {
        IsPositive = true;
    }

    // For the Unsigned Multiplication
    if (B == 0 || (*this) == 0) {
        BigInt = "0";
        Length = 1;
        IsPositive = true;
        return (*this);
    }
    std::string A_Origin = BigInt;
    BigInt.resize(Length + B.Length, '0');
    int Carry = 0;
    int Temp;
    for (int A_Digit = 0; A_Digit < Length; ++A_Digit) {
        Temp = (A_Origin[A_Digit] - 48) * (B.BigInt[0] - 48) + Carry;
        Carry = Temp / 10;
        BigInt[A_Digit] = (Temp % 10) + 48;
    }
    if (Carry != 0) {
        BigInt[Length] = Carry + 48;
    }
    BigInt[Length] = Carry + 48;
    for (int B_Digit = 1; B_Digit < B.Length; ++B_Digit) {
        Carry = 0;
        for (int A_Digit = 0; A_Digit < Length; ++A_Digit) {
            Temp = (A_Origin[A_Digit] - 48) * (B.BigInt[B_Digit] - 48)
                   + Carry + (BigInt[A_Digit + B_Digit] - 48);
            Carry = Temp / 10;
            BigInt[A_Digit + B_Digit] = (Temp % 10) + 48;
        }
        BigInt[Length + B_Digit] = Carry + 48;
    }
    if (Carry != 0) {
        Length = Length + B.Length;
    } else {
        Length = Length + B.Length - 1;
    }
    return (*this);
}

sjtu::int2048 sjtu::operator*(sjtu::int2048 A, const sjtu::int2048& B)
{
    // For the IsPositive Flag
    if ((!A.IsPositive && B.IsPositive) || (A.IsPositive && !B.IsPositive)) {
        A.IsPositive = false;
    } else {
        A.IsPositive = true;
    }

    // For the Unsigned Multiplication
    if (B == 0 || A == 0) {
        A.BigInt = "0";
        A.Length = 1;
        A.IsPositive = true;
        return A;
    }
    std::string A_Origin = A.BigInt;
    A.BigInt.resize(A.Length + B.Length, '0');
    int Carry = 0;
    int Temp;
    for (int A_Digit = 0; A_Digit < A.Length; ++A_Digit) {
        Temp = (A_Origin[A_Digit] - 48) * (B.BigInt[0] - 48) + Carry;
        Carry = Temp / 10;
        A.BigInt[A_Digit] = (Temp % 10) + 48;
    }
    if (Carry != 0) {
        A.BigInt[A.Length] = Carry + 48;
    }
    A.BigInt[A.Length] = Carry + 48;
    for (int B_Digit = 1; B_Digit < B.Length; ++B_Digit) {
        Carry = 0;
        for (int A_Digit = 0; A_Digit < A.Length; ++A_Digit) {
            Temp = (A_Origin[A_Digit] - 48) * (B.BigInt[B_Digit] - 48)
                   + Carry + (A.BigInt[A_Digit + B_Digit] - 48);
            Carry = Temp / 10;
            A.BigInt[A_Digit + B_Digit] = (Temp % 10) + 48;
        }
        A.BigInt[A.Length + B_Digit] = Carry + 48;
    }
    if (Carry != 0) {
        A.Length = A.Length + B.Length;
    } else {
        A.Length = A.Length + B.Length - 1;
    }
    return A;
}

sjtu::int2048& sjtu::int2048::operator/=(sjtu::int2048 B)
{
    // For the IsPositive Flag
    if ((!IsPositive && B.IsPositive) || (IsPositive && !B.IsPositive)) {
        IsPositive = false;
    } else {
        IsPositive = true;
    }

    // For the Unsigned Division
    if (Length < B.Length) {
        BigInt = "0";
        IsPositive = true;
        Length = 1;
        return (*this);
    }
    std::string TempA;
    TempA.resize(Length - B.Length + 1, '0');
    int ALength = Length;
    for (int i = Length - B.Length; i >= 0; --i) {
        while (!sjtu::AbsCompareLess(BigInt, i, Length, B.BigInt, B.Length)) {
            bool Carry = false;
            for (int j = 0; j < B.Length; ++j) {
                BigInt[i + j] = BigInt[i + j] - B.BigInt[j] - Carry + 48;
                if (BigInt[i + j] < 48) {
                    Carry = true;
                    BigInt[i + j] += 10;
                    continue;
                }
                Carry = false;
            }
            if (Carry) --BigInt[Length - 1];
            TempA[i] += 1;
            while (BigInt[Length - 1] == '0' && Length > 1) {
                --Length;
            }
        }
    }
    BigInt = TempA;
    Length = ALength - B.Length + 1;
    while (BigInt[Length - 1] == '0' && Length > 1) {
        --Length;
    }
    return (*this);
}

sjtu::int2048 sjtu::operator/(sjtu::int2048 A, const sjtu::int2048& B)
{
    // For the IsPositive Flag
    if ((!A.IsPositive && B.IsPositive) || (A.IsPositive && !B.IsPositive)) {
        A.IsPositive = false;
    } else {
        A.IsPositive = true;
    }

    // For the Unsigned Division
    if (A.Length < B.Length) {
        A.BigInt = "0";
        A.IsPositive = true;
        A.Length = 1;
        return A;
    }
    std::string TempA;
    TempA.resize(A.Length - B.Length + 1, '0');
    int ALength = A.Length;
    for (int i = A.Length - B.Length; i >= 0; --i) {
        while (!sjtu::AbsCompareLess(A.BigInt, i, A.Length, B.BigInt, B.Length)) {
            bool Carry = false;
            for (int j = 0; j < B.Length; ++j) {
                A.BigInt[i + j] = A.BigInt[i + j] - B.BigInt[j] - Carry + 48;
                if (A.BigInt[i + j] < 48) {
                    Carry = true;
                    A.BigInt[i + j] += 10;
                    continue;
                }
                Carry = false;
            }
            if (Carry) --A.BigInt[A.Length - 1];
            TempA[i] += 1;
            while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
                --A.Length;
            }
        }
    }
    A.BigInt = TempA;
    A.Length = ALength - B.Length + 1;
    while (A.BigInt[A.Length - 1] == '0' && A.Length > 1) {
        --A.Length;
    }
    return A;
}


std::istream& sjtu::operator>>(std::istream& InStream, sjtu::int2048& Target)
{
    std::string Temp;
    InStream >> Temp;
    Target.read(Temp);
    return InStream;
}

std::ostream& sjtu::operator<<(std::ostream& OutStream, const sjtu::int2048& Object)
{
    if (!Object.IsPositive) OutStream << "-";
    for (int i = Object.Length - 1; i >= 0; --i) {
        OutStream << Object.BigInt[i];
    }
    return OutStream;
}

bool sjtu::operator<(const sjtu::int2048& A, const sjtu::int2048& B)
{
    if (A.IsPositive && !B.IsPositive) return false;
    if (!A.IsPositive && B.IsPositive) return true;
    if (A.IsPositive) {
        if (A.Length != B.Length) return (A.Length < B.Length);
        for (int i = A.Length - 1; i >= 0; --i) {
            if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] < B.BigInt[i]);
        }
        return false;
    }
    if (A.Length != B.Length) return (A.Length > B.Length);
    for (int i = A.Length - 1; i >= 0; --i) {
        if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] > B.BigInt[i]);
    }
    return false;
}

bool sjtu::operator>(const sjtu::int2048& A, const sjtu::int2048& B)
{
    if (A.IsPositive && !B.IsPositive) return true;
    if (!A.IsPositive && B.IsPositive) return false;
    if (A.IsPositive) {
        if (A.Length != B.Length) return (A.Length > B.Length);
        for (int i = A.Length - 1; i >= 0; --i) {
            if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] > B.BigInt[i]);
        }
        return false;
    }
    if (A.Length != B.Length) return (A.Length < B.Length);
    for (int i = A.Length - 1; i >= 0; --i) {
        if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] < B.BigInt[i]);
    }
    return false;
}

bool sjtu::operator==(const sjtu::int2048& A, const sjtu::int2048& B)
{
    if ((A.IsPositive && B.IsPositive) || (!A.IsPositive && !B.IsPositive)) {
        if (A.Length != B.Length) return false;
        for (int i = 0; i < A.Length; ++i) {
            if (A.BigInt[i] != B.BigInt[i]) return false;
        }
        return true;
    }
    return false;
}

bool sjtu::operator!=(const sjtu::int2048& A, const sjtu::int2048& B)
{
    return (!(A == B));
}

bool sjtu::operator<=(const sjtu::int2048& A, const sjtu::int2048& B)
{
    if (A.IsPositive && !B.IsPositive) return false;
    if (!A.IsPositive && B.IsPositive) return true;
    if (A.IsPositive) {
        if (A.Length != B.Length) return (A.Length < B.Length);
        for (int i = A.Length - 1; i >= 0; --i) {
            if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] < B.BigInt[i]);
        }
        return true;
    }
    if (A.Length != B.Length) return (A.Length > B.Length);
    for (int i = A.Length - 1; i >= 0; --i) {
        if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] > B.BigInt[i]);
    }
    return true;
}

bool sjtu::operator>=(const sjtu::int2048& A, const sjtu::int2048& B)
{
    if (A.IsPositive && !B.IsPositive) return true;
    if (!A.IsPositive && B.IsPositive) return false;
    if (A.IsPositive) {
        if (A.Length != B.Length) return (A.Length > B.Length);
        for (int i = A.Length - 1; i >= 0; --i) {
            if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] > B.BigInt[i]);
        }
        return true;
    }
    if (A.Length != B.Length) return (A.Length < B.Length);
    for (int i = A.Length - 1; i >= 0; --i) {
        if (A.BigInt[i] != B.BigInt[i]) return (A.BigInt[i] < B.BigInt[i]);
    }
    return true;
}

bool sjtu::AbsCompareLess(const std::string& A, int Start, int End,
                          const std::string& B, int BLength)
{
    if ((End - Start) != BLength) return ((End - Start) < BLength);
    for (int i = BLength - 1; i >= 0; --i) {
        if (A[Start + i] != B[i]) return (A[Start + i] < B[i]);
    }
    return false;
}

#endif