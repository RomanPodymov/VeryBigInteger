//
//  verybiginteger.h
//  VeryBigInteger
//
//  Created by Roman Podymov on 19/01/2023.
//  Copyright © 2023 VeryBigInteger. All rights reserved.
//

#ifndef _VERYBIGINTEGER_H
#define _VERYBIGINTEGER_H

#include <bitset>
#include <algorithm>

inline bool XOR(bool a, bool b)
{
    return (!a && b) || (a && !b);
}

template <size_t N>
class VeryBigInteger
{
    public:
    VeryBigInteger() {}
    VeryBigInteger(size_t i);
    void Print();
    std::bitset<N>& getBits()
    {
        return this->bits;
    }
    operator size_t()
    {
        return this->bits.to_ulong();
    }
    private:
    std::bitset<N> bits;
    public:
    auto getBit(size_t i) -> decltype(this->bits[i])
    {
        return this->bits[i];
    }
};

template <size_t N, size_t M>
VeryBigInteger<N> Copy(VeryBigInteger<M> a)
{
    VeryBigInteger<N> result;
    size_t length = std::min(N, M);
    for (size_t i = 0 ; i < length; i++)
    {
        result.getBit(i) = a.getBit(i);
    }
    return result;
}

template <size_t K, size_t N, size_t M>
VeryBigInteger<K> operator+ (VeryBigInteger<N> a, VeryBigInteger<M> b)
{
    VeryBigInteger<K> result = Copy<K,N>(a);
    
    bool isOverFlow = false;
    size_t i = 0;
    size_t limit = std::min(N, M);
    for (; i < limit; i++)
    {
        bool bitFromA = a.getBits().operator[](i);
        bool bitFromB = b.getBits().operator[](i);
        result.getBits().operator[](i) = XOR(bitFromA, bitFromB);
        bool currentOverFlow = false;
        if (!result.getBits().operator[](i) && (bitFromA || bitFromB))
        {
            currentOverFlow = true;
        }
        if (isOverFlow)
        {
            result.getBits().operator[](i) = XOR(isOverFlow, result.getBits().operator[](i));
            if (!currentOverFlow && result.getBits().operator[](i) && (!(XOR(result.getBits().operator[](i), isOverFlow))))
            {
                isOverFlow = false;
            }
        }
        else
        {
            isOverFlow = currentOverFlow;
        }
    }
    if (isOverFlow)
    {
        result.getBits().operator[](i) = true;
    }
    
    return result;
}

template <size_t N>
VeryBigInteger<N> operator- (VeryBigInteger<N> a, VeryBigInteger<N> b)
{
    VeryBigInteger<N> result(0);
    
    for (size_t i = 0; i < N; i++)
    {
        bool bitFromA = a.getBits().operator[](i);
        bool bitFromB = b.getBits().operator[](i);
        result.getBits().operator[](i) = bitFromA != bitFromB;
        if (!bitFromA && bitFromB)
        {
            bool noBitToOverflow = true;
            for (size_t j = i + 1; j < N; j++)
            {
                if (a.getBits().operator[](j))
                {
                    a.getBits().operator[](j) = false;
                    noBitToOverflow = false;
                    break;
                }
                else
                {
                    a.getBits().operator[](j) = true;
                }
            }
            if (noBitToOverflow)
            {
                result.getBits().operator[](i) = false;
            }
        }
    }
    
    return result;
}

template <size_t K, size_t N>
VeryBigInteger<K> operator* (VeryBigInteger<N> a, size_t b)
{
    VeryBigInteger<K> result = Copy<K,N>(a);
    
    for (size_t i = 1; i < b; i++)
    {
        result = operator+ <K> (result, a); 
    }

    return result;	
}

template <size_t K, size_t N, size_t M>
VeryBigInteger<K> operator* (VeryBigInteger<N> a, VeryBigInteger<M> b)
{
    VeryBigInteger<K> result = Copy<K,N>(a);
    
    for (size_t i = 1; i < b.getBits().to_ulong(); i++)
    {
        result = operator+ <K> (result, a); 
    }

    return result;	
}

template <size_t K>
VeryBigInteger<K> SlowPow (size_t a, size_t b)
{
    VeryBigInteger<K> result(a);
    
    for (size_t i = 1; i < b; i++)
    {
        result = operator* <K> (result, a); 
    }

    return result;	
}

template <size_t K>
VeryBigInteger<K> FastPow (size_t a, size_t b)
{
    VeryBigInteger<K> result(1);
    VeryBigInteger<K> bigA(a);

    while (b != 0)
    {
        if (b % 2 != 0)
        {
            result = operator*<K>(result,bigA);
            b--;
        }
        bigA = operator*<K>(bigA,bigA);
        b >>= 1;
    }

    return result;
}

template <size_t N>
static VeryBigInteger<N> moveToLeft(size_t value, size_t n)
{
    VeryBigInteger<N> result(value);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = N - 1; j >= 1; j--)
        {
            result.getBits().operator[](j) = result.getBits().operator[](j - 1);
        }
        result.getBits().operator[](0) = false; 
    }

    return result;
}

template <size_t N>
VeryBigInteger<N> operator/ (VeryBigInteger<N> a, size_t b)
{
    VeryBigInteger<N> result(0);
    VeryBigInteger<N> currentNumber(0);
    VeryBigInteger<N> afterDivide;
    VeryBigInteger<N> tmpDiff;

    for (size_t i = 1; i <= N; i++)
    {
        currentNumber = getFirstNBits(a,i);
        result.getBits().operator[](N - i) = currentNumber.getBits().to_ulong() / b;
        afterDivide = moveToLeft<N>((result.getBits().operator[](N - i) ? 1 : 0) * b, N - i);
        currentNumber = moveToLeft<N>(currentNumber.getBits().to_ulong(), N - i);
        tmpDiff = currentNumber - afterDivide;
        CopyDiff(tmpDiff, a, N - i);
    }

    return result;	
}

template <size_t N>
static void CopyDiff(VeryBigInteger<N> source, VeryBigInteger<N> &out, size_t i)
{
    for (size_t j = i; j < N; j++)
    {
        out.getBits().operator[](j) = source.getBits().operator[](j);
    }
}

template <size_t N>
size_t operator% (VeryBigInteger<N> a, size_t b)
{
    VeryBigInteger<N> tmp = operator/<N>(a,b);
    VeryBigInteger<N> mult = operator*<N>(tmp,b);
    VeryBigInteger<N> diff = operator-<N>(a,mult);
    return diff.getBits().to_ulong();
}

template <size_t N, size_t K>
static VeryBigInteger<N> getFirstNBits(VeryBigInteger<K> a)
{
    VeryBigInteger<N> result;

    for (size_t i = 0; i < K; i++)
    {
        result.getBits().operator[](K - i - 1) = a.getBits().operator[](N - i - 1);
    }

    return result;
}

template <size_t N>
static VeryBigInteger<N> getFirstNBits(VeryBigInteger<N> a, size_t n)
{
    VeryBigInteger<N> result;

    for (size_t i = 0; i < n; i++)
    {
        result.getBits().operator[](n - i - 1) = a.getBits().operator[](N - i - 1);
    }

    return result;
}

template <size_t N>
VeryBigInteger<N>::VeryBigInteger(size_t i)
{
    bits = std::bitset<N>(i);
}

template <size_t N>
void VeryBigInteger<N>::Print()
{
    for (int i = N - 1; i>=0; i--)
    {
        printf("%d", this->bits.operator[](i) ? 1 : 0);
    }
    printf("\n");
}

#endif
