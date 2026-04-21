#include "compare.hpp"

#include "number.hpp"
#include "../CalculationError.hpp"

#include <algorithm>

int CompareNumbers(Number& num1, Number& num2, bool ignoreSign, int shiftSecond) {
    // If num1 > num2 returns  1
    // If num1 = num2 returns  0
    // If num1 < num2 returns -1

    if (num1.GetDigits().empty()) {
        throw CalculationError("Compare operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }
    if (num2.GetDigits().empty()) {
        throw CalculationError("Compare operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }

    num1.CorrectForSignificance();
    num2.CorrectForSignificance();

    const std::vector<int>& digits1 = num1.GetDigits();
    const std::vector<int>& digits2 = num2.GetDigits();
    const int size1 = static_cast<int>(digits1.size());
    const int size2 = static_cast<int>(digits2.size());
    const int exponent1 = num1.GetExponent();
    const int secondExponent = num2.GetExponent() + shiftSecond;
    const bool num1Negative = num1.GetIsNegative();
    const bool num2Negative = num2.GetIsNegative();

    const bool num1IsZero = size1 == 1 && digits1[0] == 0;
    const bool num2IsZero = size2 == 1 && digits2[0] == 0;

    // Zero has no meaningful exponent. Handle it explicitly before exponent-based ordering.
    if (num1IsZero && num2IsZero) {
        return 0;
    }
    if (num1IsZero) {
        if (ignoreSign) {
            return -1;
        }
        return num2Negative ? 1 : -1;
    }
    if (num2IsZero) {
        if (ignoreSign) {
            return 1;
        }
        return num1Negative ? -1 : 1;
    }

    // Negative number-combination to filter out
    if (!ignoreSign) {
        if (num1Negative && !num2Negative) {
            return -1;
        }
        if (!num1Negative && num2Negative) {
            return 1;
        }
    }

    const bool signedCompareNegative = !ignoreSign && num1Negative;
    const int greaterResult = signedCompareNegative ? -1 : 1;
    const int smallerResult = signedCompareNegative ? 1 : -1;
    

    // Check for exponent
    if (exponent1 > secondExponent) {
        return greaterResult;
    } else if (exponent1 < secondExponent) {
        return smallerResult;
    }

    // Check for digits
    const int maxDigits = std::max(size1, size2);
    for (int i = 0; i < maxDigits; i++) {
        int digit1 = i < size1 ? digits1[i] : 0;
        int digit2 = i < size2 ? digits2[i] : 0;

        if (digit1 > digit2) {
            return greaterResult;
        } else if (digit1 < digit2) {
            return smallerResult;
        }
    }

    return 0;

}