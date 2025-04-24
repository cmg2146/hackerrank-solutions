// Solves: https://www.hackerrank.com/challenges/cpp-variadics/problem?isFullScreen=true

#include <iostream>
using namespace std;

template <bool Bit, bool... BitsToRight>
struct ReverseBits {
    enum
    { 
        value = Bit + (2 * ReverseBits<BitsToRight...>::value)
    };
};

// specialization for when we are processing the last bit
template <bool Bit>
struct ReverseBits<Bit> {
    enum
    {
        value = Bit
    };
};

// a solution which calculates, at compile time, the integer value of a variadic list of bits in reverse order
// i.e. reversed_binary_value<1,0,1,1>() returns 13
template <bool... digits>
static constexpr int reversed_binary_value()
{
    return ReverseBits<digits...>::value;
}
