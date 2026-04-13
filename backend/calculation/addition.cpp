#include "addition.hpp"
#include "compare.hpp"

Number Addition::Calculate(Number& a, Number& b, int shiftB) {
    Number result(a.GetMaxSignificant());

    // the calculations can be incorrect with shiftB if: shifted_b > a

    // Copy a to result

    int carry = 0;

    


    // Base Addition algorithm
    if (a.GetIsNegative() == b.GetIsNegative()) {
        result.SetDigits(a.GetDigits());
        result.SetExponent(a.GetExponent());
        result.SetNegative(a.GetIsNegative());

        // Add b to result
        for (int i = b.GetDigits().size() - 1; i >= 0; i--) {
            int digit = b.GetDigits().at(i);
            int exponent = b.GetExponent() - i + shiftB;

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
            int exponent = b.GetExponent() + i + shiftB;
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
    } else {
        Number numNeg = a.GetIsNegative() ? a : b;
        Number numPos = a.GetIsNegative() ? b : a;

        Number first(numNeg.GetMaxSignificant());
        Number second(numNeg.GetMaxSignificant());

        if (CompareNumbers(numPos, numNeg, true) >= 0) {
            first = numPos;
            second = numNeg;
            result.SetNegative(false);
        } else {
            first = numNeg;
            second = numPos;
            result.SetNegative(true);
        }

        // Calculate the Subtraction
        result.SetDigits(first.GetDigits());
        result.SetExponent(first.GetExponent());
        carry = 0;

        for (int i = second.GetDigits().size() - 1; i >= 0; i--) {
            int digit = second.GetDigits().at(i);
            int exponent = second.GetExponent() - i + shiftB;

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
            int exponent = second.GetExponent() + i + shiftB;
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
    } 


    result.CorrectForSignificance();
    return result;
}