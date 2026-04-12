#include "addition.hpp"

Number Addition::Calculate(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    // Copy a to result
    result.SetDigits(a.GetDigits());
    result.SetExponent(a.GetExponent());
    result.SetNegative(a.GetIsNegative());

    int carry = 0;

    // Add b to result
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

        // Add digit to result
        int sum = result.GetDigits().at(resultIndex) + digit + carry;
        result.GetDigits().at(resultIndex) = sum % 10;
        carry = sum / 10;
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

        int sum = result.GetDigits().at(resultIndex) + carry;
        result.GetDigits().at(resultIndex) = sum % 10;
        carry = sum / 10;
    }


    result.CorrectForSignificance();
    return result;
}