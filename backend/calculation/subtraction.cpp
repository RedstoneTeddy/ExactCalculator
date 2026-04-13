#include "subtraction.hpp"
#include "addition.hpp"

Number Subtraction::Calculate(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());
    
    Number b_neg(b.GetMaxSignificant());
    b_neg.SetNegative(!b.GetIsNegative());

    result = Addition::Calculate(a, b_neg);

    result.CorrectForSignificance();
    return result;
}