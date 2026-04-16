#include "addition.hpp"
#include "compare.hpp"

#include <algorithm>

namespace {
int DigitAtExponent(Number& num, int exponent) {
    int index = num.GetExponent() - exponent;
    if (index < 0 || index >= static_cast<int>(num.GetDigits().size())) {
        return 0;
    }
    return num.GetDigits()[index];
}

Number AddPositiveFast(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    int topExp = std::max(a.GetExponent(), b.GetExponent());
    int aBottomExp = a.GetExponent() - static_cast<int>(a.GetDigits().size()) + 1;
    int bBottomExp = b.GetExponent() - static_cast<int>(b.GetDigits().size()) + 1;
    int bottomExp = std::min(aBottomExp, bBottomExp);

    std::vector<int> sumDigitsLeastFirst;
    sumDigitsLeastFirst.reserve(topExp - bottomExp + 2);

    int carry = 0;
    for (int exponent = bottomExp; exponent <= topExp; exponent++) {
        int sum = DigitAtExponent(a, exponent) + DigitAtExponent(b, exponent) + carry;
        sumDigitsLeastFirst.push_back(sum % 10);
        carry = sum / 10;
    }

    int resultExponent = topExp;
    if (carry > 0) {
        sumDigitsLeastFirst.push_back(carry);
        resultExponent++;
    }

    std::vector<int> resultDigits;
    resultDigits.reserve(sumDigitsLeastFirst.size());
    for (int i = static_cast<int>(sumDigitsLeastFirst.size()) - 1; i >= 0; i--) {
        resultDigits.push_back(sumDigitsLeastFirst[i]);
    }

    result.SetNumber(false, resultDigits, resultExponent);
    result.CorrectForSignificance();
    return result;
}
} // namespace

Number Addition::Calculate(Number& a, Number& b) {
    return Calculate(a, b, 0);
}

Number Addition::Calculate(Number& a, Number& b, int shiftB) {
    Number result(a.GetMaxSignificant());

    a.CorrectForSignificance();
    b.CorrectForSignificance();

    bool aIsZero = a.GetDigits().size() == 1 && a.GetDigits()[0] == 0;
    bool bIsZero = b.GetDigits().size() == 1 && b.GetDigits()[0] == 0;

    if (aIsZero) {
        return b;
    }
    if (bIsZero) {
        return a;
    }

    // Hot path in exponent/division internals: positive numbers without shift.
    if (shiftB == 0 && !a.GetIsNegative() && !b.GetIsNegative()) {
        return AddPositiveFast(a, b);
    }

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

        if (CompareNumbers(numPos, numNeg, true, shiftB) >= 0) {
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

            // Optimization
            if (resultIndex > a.GetMaxSignificant()+1 || resultIndex > b.GetMaxSignificant()+1) {
                continue;
            }

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