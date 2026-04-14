#include "compare.hpp"

#include "number.hpp"

int CompareNumbers(Number& num1, Number& num2, bool ignoreSign, int shiftSecond) {
    // If num1 > num2 returns  1
    // If num1 = num2 returns  0
    // If num1 < num2 returns -1

    num1.CorrectForSignificance();
    num2.CorrectForSignificance();

    int second_exponent = num2.GetExponent() + shiftSecond;

    bool num1IsZero = num1.GetDigits().size() == 1 && num1.GetDigits()[0] == 0;
    bool num2IsZero = num2.GetDigits().size() == 1 && num2.GetDigits()[0] == 0;

    // Zero has no meaningful exponent. Handle it explicitly before exponent-based ordering.
    if (num1IsZero && num2IsZero) {
        return 0;
    }
    if (num1IsZero) {
        if (ignoreSign) {
            return -1;
        }
        return num2.GetIsNegative() ? 1 : -1;
    }
    if (num2IsZero) {
        if (ignoreSign) {
            return 1;
        }
        return num1.GetIsNegative() ? -1 : 1;
    }

    // Check for equality (optionally ignoring sign)
    bool signsEqual = num1.GetIsNegative() == num2.GetIsNegative();
    if ((ignoreSign || signsEqual) && num1.GetDigits() == num2.GetDigits() && num1.GetExponent() == second_exponent) {
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
    if (num1.GetExponent() > second_exponent) {
        if (ignoreSign) {
            return 1;
        }
        return num1.GetIsNegative() ? -1 : 1;
    } else if (num1.GetExponent() < second_exponent) {
        if (ignoreSign) {
            return -1;
        }
        return num1.GetIsNegative() ? 1 : -1;
    }

    // Check for digits
    for (int i = 0; i < std::max(num1.GetDigits().size(), num2.GetDigits().size()); i++) {
        int digit1 = i < num1.GetDigits().size() ? num1.GetDigits().at(i) : 0;
        int digit2 = i < num2.GetDigits().size() ? num2.GetDigits().at(i) : 0;

        if (digit1 > digit2) {
            if (ignoreSign) {
                return 1;
            }
            return num1.GetIsNegative() ? -1 : 1;
        } else if (digit1 < digit2) {
            if (ignoreSign) {
                return -1;
            }
            return num1.GetIsNegative() ? 1 : -1;
        }
    }

    return 0;

}