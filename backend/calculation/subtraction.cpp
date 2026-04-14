#include <iostream>
#include "subtraction.hpp"
#include "addition.hpp"

Number Subtraction::Calculate(Number& a, Number& b) {
    return Calculate(a, b, 0);
}

Number Subtraction::Calculate(Number& a, Number& b, int shiftB) {
    Number result(a.GetMaxSignificant());
    
    Number b_neg(b.GetMaxSignificant());
    b_neg.SetNegative(!b.GetIsNegative());
    b_neg.SetDigits(b.GetDigits());
    b_neg.SetExponent(b.GetExponent());

    b_neg.CorrectForSignificance();

    result = Addition::Calculate(a, b_neg, shiftB);

    result.CorrectForSignificance();
    return result;
}