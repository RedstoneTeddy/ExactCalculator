#include "compare.hpp"

#include "number.hpp"

int CompareNumbers(Number& num1, Number& num2, bool ignoreSign) {
    // If num1 > num2 returns  1
    // If num1 = num2 returns  0
    // If num1 < num2 returns -1

    num1.CorrectForSignificance();
    num2.CorrectForSignificance();

    // Check for equality
    if (num1.GetIsNegative() == num2.GetIsNegative() && num1.GetDigits() == num2.GetDigits() && num1.GetExponent() == num2.GetExponent()) {
        return 0;
    }

    // Negative number-combination to filter out
    if (!ignoreSign) {
        if (num1.GetIsNegative() && !num2.GetIsNegative()) {
            return -1;
        }
        if (!num1.GetIsNegative() && num2.GetIsNegative()) {
            return 1;
        }
    }
    

    // Check for exponent
    if (num1.GetExponent() > num2.GetExponent()) {
        return num1.GetIsNegative() ? -1 : 1;
    } else if (num1.GetExponent() < num2.GetExponent()) {
        return num1.GetIsNegative() ? 1 : -1;
    }

    // Check for digits
    for (int i = 0; i < std::max(num1.GetDigits().size(), num2.GetDigits().size()); i++) {
        int digit1 = i < num1.GetDigits().size() ? num1.GetDigits().at(i) : 0;
        int digit2 = i < num2.GetDigits().size() ? num2.GetDigits().at(i) : 0;

        if (digit1 > digit2) {
            return num1.GetIsNegative() ? -1 : 1;
        } else if (digit1 < digit2) {
            return num1.GetIsNegative() ? 1 : -1;
        }
    }

    return 0;

}