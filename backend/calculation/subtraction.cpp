#include "subtraction.hpp"

Number Subtraction::Calculate(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    // Copy a to result
    result.SetDigits(a.GetDigits());
    result.SetExponent(a.GetExponent());
    result.SetNegative(a.GetIsNegative());

    int carry = 0;

    // Subtract b from result
    for (int i = b.GetDigits().size() - 1; i >= 0; i--) {
        int digit = b.GetDigits().at(i);
        int exponent = b.GetExponent() - i;

        // Adjust result to fit new digit
        int resultIndex = result.GetExponent() - exponent;
        if (resultIndex < 0) {
            // Need to add leading zeros to result
            for (int j = 0; j < -resultIndex; j++) {
                result.GetDigits().insert(result.GetDigits().begin(), 0);
            }
            result.SetExponent(exponent);
            resultIndex = 0;
        } else if (resultIndex >= result.GetDigits().size()) {
            // Need to add trailing zeros to result
            for (int j = result.GetDigits().size(); j <= resultIndex; j++) {
                result.GetDigits().push_back(0);
            }
        }

        // Subtract digit from result
        int diff = result.GetDigits().at(resultIndex) - digit - carry;
        if (diff < 0) {
            diff += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result.GetDigits().at(resultIndex) = diff;
    }

    // Handle carry for remaining digits
    int i = 0;
    while (carry > 0) {
        i++;
        int exponent = b.GetExponent() + i;
        int resultIndex = result.GetExponent() - exponent;
        if (resultIndex < 0) {
            // Need to add leading zeros to result
            for (int j = 0; j < -resultIndex; j++) {
                result.GetDigits().insert(result.GetDigits().begin(), 0);
            }
            result.SetExponent(exponent);
            resultIndex = 0;
        } else if (resultIndex >= result.GetDigits().size()) {
            // Need to add trailing zeros to result
            for (int j = result.GetDigits().size(); j <= resultIndex; j++) {
                result.GetDigits().push_back(0);
            }
        }

        int diff = result.GetDigits().at(resultIndex) - carry;
        if (diff < 0) {
            diff += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result.GetDigits().at(resultIndex) = diff;
    }


    result.CorrectForSignificance();
    return result;
}