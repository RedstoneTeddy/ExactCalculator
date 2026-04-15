#include "division.hpp"
#include "base_operation.hpp"
#include "number.hpp"
#include "multiplication.hpp"
#include "compare.hpp"

#include <vector>


Number Division::Calculate(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    Number a_leftover(a.GetMaxSignificant());
    a_leftover.SetNumber(false, a.GetDigits(), a.GetExponent());

    Number posB(a.GetMaxSignificant());
    posB.SetNumber(false, b.GetDigits(), b.GetExponent());

    std::vector<Number> divisors;
    divisors.push_back(posB);
    for (int i = 1; i < 9; i++) {
        Number nextDivisor = Addition::Calculate(divisors[i - 1], posB);
        divisors.push_back(nextDivisor);
    }
    
    // Do the division
    int resultDigit, shift;
    int i_limit = result.GetMaxSignificant() + 1;
    for (int i = 0; i <= i_limit; i++) {
        shift = a.GetExponent() - b.GetExponent() - i;
        resultDigit = 0;

        for (int j = 8; j >= 0; j--) {
            if (CompareNumbers(a_leftover, divisors[j], true, shift) >= 0) {
                resultDigit = j + 1;
                break;
            }
        }

        if (resultDigit > 0) {
            a_leftover = Subtraction::Calculate(a_leftover, divisors[resultDigit - 1], shift);
        }
        result.GetDigits().push_back(resultDigit);

        if (a_leftover.GetDigits().size() == 1 && a_leftover.GetDigits()[0] == 0) {
            break;
        }

    }


    result.SetExponent(a.GetExponent() - b.GetExponent() + 1);
    result.SetNegative(a.GetIsNegative() != b.GetIsNegative());
    result.CorrectForSignificance();
    return result;
}