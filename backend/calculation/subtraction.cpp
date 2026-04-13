#include "subtraction.hpp"
#include "addition.hpp"

Number Subtraction::Calculate(Number& a, Number& b, int shiftB) {
    Number result(a.GetMaxSignificant());
    
    Number b_neg(b.GetMaxSignificant());
    b_neg.SetNegative(!b.GetIsNegative());

    result = Addition::Calculate(a, b_neg, shiftB);

    result.CorrectForSignificance();
    return result;
}