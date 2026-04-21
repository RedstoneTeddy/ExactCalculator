#ifndef EXPONENT_HPP
#define EXPONENT_HPP

#include "division.hpp"
#include "number.hpp"
#include "compare.hpp"
#include <vector>

class Exponent : public Division{
public:
    int rootSignificant;
    Exponent(int rootSignificant) {
        this->rootSignificant = rootSignificant;
    }
    Number Calculate(Number& a, Number& b) override;
    Number Calculate(Number& a, Number& b, int rootSignificant);
};

bool Exp_NumberHasComma(Number& number);
bool Exp_NumberIsSmallerThanOne(Number& number);
bool Exp_NumberIsZero(Number& number);


Number Exp_Average(Number& a, Number& b);

Number Exp_SquareRoot(Number& number, int rootSignificant);

Number Exp_Round(Number& number, int significant);

#endif // EXPONENT_HPP