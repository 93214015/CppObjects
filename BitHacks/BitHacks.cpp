// BitHacks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


//the bits number needed for moving the most significant bit to the least significant bit
constexpr int SHIFTING_COUNT = (sizeof(int) * 8) - 1;

// Negating -- This function wouldn't be needed directly. the negeting instrinsic instruction is available for processors.
// the goal of writing of this functionality is for better conprehension of Abs() and other functions.
inline int Negate(int x)
{
    //return (~x) + 1;
    return (x ^ -1) + 1 ; //(x xor 0xFFFFFFFF) - (-1)
}

// Absolute Value
inline int Abs(int x)
{
    const int m = x >> SHIFTING_COUNT;

    return (x ^ m) - m;
}

// Sign Function
inline int Sgn(int x)
{
    return (x >> SHIFTING_COUNT) - (-x >> SHIFTING_COUNT);
}

// Some Predicates
void Predicates(int x, int a)
{
    // x = (a == 0)
    x = (unsigned)~(a | -a) >> SHIFTING_COUNT;

    // x = (a!=0)
    x = (unsigned)(a | -a) >> SHIFTING_COUNT;

    // x = (a > 0)
    x = (unsigned)(-a) >> SHIFTING_COUNT;

    // x = (a < 0)
    x = (unsigned)(a) >> SHIFTING_COUNT;

    // x = (a>=0)
    x = (unsigned)(~a) >> SHIFTING_COUNT;

    // x = (a<=0)
    x = (unsigned)(a - 1) >> SHIFTING_COUNT;
}

// Conditional Increment or Decrement -- we have conditional operation without any branching
void ConditionalIncDec(int x, int a)
{
    //here I've used a > 0 condition for instance

    // Increment -- by mark the value as an unsigned we would have normal right shift operation(shr) which doesn't duplicate sign bit.
    // so, here we'll get the number 0x00000001 if the sign bit would be 1.
    x += ((unsigned)-a >> SHIFTING_COUNT);

    // Decrement -- with keep the value as a signed number we would have arithmetic right shift operation(sar) which duplicate the sign bit.
    // Then, the result of the operation if the sign bit would be 1 is 0xFFFFFFFF which is a -1 number.
    x += (-a >> SHIFTING_COUNT);
}

// Conditional Addition or Subtraction
void ConditionalAddSub(int x, int y, int a)
{
    // I've used a <= 0 condition for instance

    // Addition
    x += (y & ((unsigned) (a - 1) >> SHIFTING_COUNT));

    // Subtraction
    x -= (y & ((unsigned)(a - 1) >> SHIFTING_COUNT));
}

// Incrementing or Decrementing the x and then keep the result as a modulo of N
template<int N>
void IncrementDecremetnModN(int x)
{
    //Increment
    x++;
    x = (x & ((x - N) >> SHIFTING_COUNT);

    //Decrement
    x--;
    x += (N & (x >> SHIFTING_COUNT));
}

// Clamping To Zero

inline int MinZero(int x)
{
    return (x & (x >> SHIFTING_COUNT));
}

inline int MaxZero(int x)
{
    return (x & (~x >> SHIFTING_COUNT));
}

// Minimum or Maximum -- the important point we should consider here is this function result will be correct in the cases we could assume
// the subtraction of two numbers would not overflow or underflow.

inline int Minimum(int x, int y)
{
    int a = x - y;
    return (x - (a & (~a >> SHIFTING_COUNT)));
}

inline int Maximum(int x, int y)
{
    int a = x - y;
    return (x - (a & (a >> SHIFTING_COUNT)));
}

// Logic Formulas
void LogicFormulas(int x)
{

    // Clear the lowest 1 bit
    x = x & (x - 1);

    // Set the lowest 0 bit
    x = x | (x + 1);

    //Set all bit to right of the lowest 1 bit
    x = x | (x - 1);

    // Clear all bits to right of the lowest 0 bit
    x = x & (x + 1);

    //Extract the lowest 1 bit
    x = x & -x;

    //Extract the lowest 0 bit (as 1 bit)
    x = ~x & (x + 1);

}


int main()
{
    std::cout << "Hello World!\n";
}

