#include <iostream>
#include "subtraction.hpp"
#include "addition.hpp"
#include "compare.hpp"
#include "../CalculationError.hpp"

#include <algorithm>

namespace {
int DigitAtExponent(Number& num, int exponent) {
    int index = num.GetExponent() - exponent;
    if (index < 0 || index >= static_cast<int>(num.GetDigits().size())) {
        return 0;
    }
    return num.GetDigits()[index];
}

Number SubtractPositiveFast(Number& larger, Number& smaller, bool negativeResult) {
    Number result(larger.GetMaxSignificant());

    int topExp = larger.GetExponent();
    int largerBottomExp = larger.GetExponent() - static_cast<int>(larger.GetDigits().size()) + 1;
    int smallerBottomExp = smaller.GetExponent() - static_cast<int>(smaller.GetDigits().size()) + 1;
    int bottomExp = std::min(largerBottomExp, smallerBottomExp);

    std::vector<int> diffDigitsLeastFirst;
    diffDigitsLeastFirst.reserve(topExp - bottomExp + 1);

    int borrow = 0;
    for (int exponent = bottomExp; exponent <= topExp; exponent++) {
        int diff = DigitAtExponent(larger, exponent) - DigitAtExponent(smaller, exponent) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        diffDigitsLeastFirst.push_back(diff);
    }

    std::vector<int> resultDigits;
    resultDigits.reserve(diffDigitsLeastFirst.size());
    for (int i = static_cast<int>(diffDigitsLeastFirst.size()) - 1; i >= 0; i--) {
        resultDigits.push_back(diffDigitsLeastFirst[i]);
    }

    result.SetNumber(negativeResult, resultDigits, topExp);
    result.CorrectForSignificance();
    return result;
}
} // namespace

Number Subtraction::Calculate(Number& a, Number& b) {
    return Calculate(a, b, 0);
}

Number Subtraction::Calculate(Number& a, Number& b, int shiftB) {
    Number result(a.GetMaxSignificant());

    if (a.GetDigits().empty()) {
        throw CalculationError("Subtraction operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }
    if (b.GetDigits().empty()) {
        throw CalculationError("Subtraction operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }

    if (shiftB == 0 && !a.GetIsNegative() && !b.GetIsNegative()) {
        int cmp = CompareNumbers(a, b, true);
        if (cmp == 0) {
            return result;
        }
        if (cmp > 0) {
            return SubtractPositiveFast(a, b, false);
        }
        return SubtractPositiveFast(b, a, true);
    }
    
    Number b_neg(b.GetMaxSignificant());
    b_neg.SetNegative(!b.GetIsNegative());
    b_neg.SetDigits(b.GetDigits());
    b_neg.SetExponent(b.GetExponent());

    b_neg.CorrectForSignificance();

    result = Addition::Calculate(a, b_neg, shiftB);

    result.CorrectForSignificance();
    return result;
}