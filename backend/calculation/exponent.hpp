#ifndef EXPONENT_HPP
#define EXPONENT_HPP

#include "division.hpp"
#include "number.hpp"
#include "compare.hpp"
#include <vector>

class Exponent : public Division{
public:
    Number Calculate(Number& a, Number& b) override;
    Number Calculate(Number& a, Number& b, int rootSignificant);
};

bool NumberHasComma(Number& number);
bool NumberIsSmallerThanOne(Number& number);
bool NumberIsZero(Number& number);

Number Average(Number& a, Number& b);

Number SquareRoot(Number& number, int rootSignificant);

Number Round(Number& number, int significant);

#endif // EXPONENT_HPP