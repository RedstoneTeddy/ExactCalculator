#include "Exponent.hpp"
#include "division.hpp"
#include "addition.hpp"
#include "number.hpp"
#include "compare.hpp"
#include <algorithm>
#include <limits>
#include <vector>

namespace {
bool IsZeroFast(Number& number) {
    return number.GetDigits().size() == 1 && number.GetDigits()[0] == 0;
}

bool IsOneFast(Number& number) {
    return !number.GetIsNegative() && number.GetExponent() == 0 && number.GetDigits().size() == 1 && number.GetDigits()[0] == 1;
}

Number HalfNonNegative(Number& value) {
    Number result(value.GetMaxSignificant());

    std::vector<int> halvedDigits;
    halvedDigits.reserve(value.GetMaxSignificant());

    int remainder = 0;
    for (int digit : value.GetDigits()) {
        int current = remainder * 10 + digit;
        halvedDigits.push_back(current / 2);
        remainder = current % 2;
    }

    while (remainder != 0 && static_cast<int>(halvedDigits.size()) < value.GetMaxSignificant()) {
        int current = remainder * 10;
        halvedDigits.push_back(current / 2);
        remainder = current % 2;
    }

    result.SetNumber(false, halvedDigits, value.GetExponent());
    result.CorrectForSignificance();
    return result;
}

bool TryGetNonNegativeInteger(Number& number, int& value) {
    // Integer if the least-significant represented exponent is >= 0.
    int bottomExponent = number.GetExponent() - static_cast<int>(number.GetDigits().size()) + 1;
    if (bottomExponent < 0 || number.GetIsNegative()) {
        return false;
    }

    long long parsed = 0;
    for (int digit : number.GetDigits()) {
        if (parsed > (std::numeric_limits<int>::max() - digit) / 10) {
            return false;
        }
        parsed = parsed * 10 + digit;
    }

    for (int i = 0; i < bottomExponent; i++) {
        if (parsed > std::numeric_limits<int>::max() / 10) {
            return false;
        }
        parsed *= 10;
    }

    value = static_cast<int>(parsed);
    return true;
}

Number PowerBySquaring(Number& base, int exponent) {
    Number result(base.GetMaxSignificant());
    result.SetNumber(false, {1}, 0);

    Number factor(base.GetMaxSignificant());
    factor.SetNumber(base.GetIsNegative(), base.GetDigits(), base.GetExponent());

    Multiplication multiplication;
    while (exponent > 0) {
        if ((exponent & 1) != 0) {
            result = multiplication.Calculate(result, factor);
        }
        exponent >>= 1;
        if (exponent > 0) {
            factor = multiplication.Calculate(factor, factor);
        }
    }

    return result;
}
} // namespace

Number Exponent::Calculate(Number& a, Number& b) {
    return Calculate(a, b, this->rootSignificant);
}




Number Exponent::Calculate(Number& a, Number& b, int rootSignificant) {
    Number result(a.GetMaxSignificant());
    result.SetNumber(false, {1}, 0); // Start with 1

    int finalSignificant = rootSignificant;
    rootSignificant = int(rootSignificant * 1.1);
    
    Number one(a.GetMaxSignificant());
    one.SetNumber(false, {1}, 0);

    a.CorrectForSignificance();
    b.CorrectForSignificance();

    if (IsZeroFast(b)) {
        return one;
    }
    if (IsOneFast(a)) {
        return one;
    }
    if (IsZeroFast(a)) {
        return Number(a.GetMaxSignificant());
    }

    Number b_leftover(rootSignificant);
    b_leftover.SetNumber(false, b.GetDigits(), b.GetExponent());
    b_leftover.SetMaxSignificant(rootSignificant);
    b_leftover.CorrectForSignificance();


    // Base exponent (before comma)
    int integerExponent = 0;
    if (TryGetNonNegativeInteger(b_leftover, integerExponent)) {
        result = PowerBySquaring(a, integerExponent);
        b_leftover.SetNumber(false, {0}, 0);
    } else {
        while (!IsZeroFast(b_leftover) && b_leftover.GetExponent() >= 0) {
            result = Multiplication::Calculate(result, a);
            b_leftover = Subtraction::Calculate(b_leftover, one);
        }
    }


    // Root-exponent (after comma)
    if (!IsZeroFast(b_leftover)) {
        if (a.GetIsNegative()) {
            return Number(a.GetMaxSignificant()); // Return 0 if trying to calculate root of negative number
        }

        Number current_root(rootSignificant);
        Number current_divider(rootSignificant);
        Number numZero(rootSignificant);
        numZero.SetNumber(false, {0}, 0);

        current_root.SetNumber(false, a.GetDigits(), a.GetExponent());
        current_root.CorrectForSignificance();

        current_divider.SetNumber(false, {1}, 0);

        bool run = true;
        int i = 0;
        int max_i = rootSignificant * 4;
        while (run) {
            current_divider = HalfNonNegative(current_divider);

            // If divider is below b_leftover's representable subtraction granularity,
            // additional iterations cannot change b_leftover at current precision.
            int subtractionNoOpExponent = b_leftover.GetExponent() - b_leftover.GetMaxSignificant() - 1;
            if (current_divider.GetExponent() < subtractionNoOpExponent) {
                break;
            }

            Number new_root = Exp_SquareRoot(current_root, rootSignificant);

            if (CompareNumbers(b_leftover, current_divider) >= 0) {
                result = Multiplication::Calculate(result, new_root);
                b_leftover = Subtraction::Calculate(b_leftover, current_divider);
            }

            // Check if calculation is finished
            run = true;
            if (IsZeroFast(b_leftover)) {
                run = false;
            }

            if (IsZeroFast(current_divider)) {
                run = false;
            }

            if (CompareNumbers(new_root, current_root) == 0) {
                run = false;
            }
            current_root = new_root;

            i++;
            if (i > max_i) {
                run = false;
            }

        }




    }




    // Inverse of result if exponent is negative
    if (b.GetIsNegative()) {
        if (IsZeroFast(result)) {
            return Number(a.GetMaxSignificant()); // Return 0 if trying to calculate inverse of 0
        }
        result = Division::Calculate(one, result);
    }
    result = Exp_Round(result, finalSignificant); // Round result to prevent precision issues from propagating further in the calculation
    return result;
}




bool Exp_NumberHasComma(Number& number) {
    number.CorrectForSignificance();
    return 0 > number.GetExponent() - number.GetDigits().size() + 1;
}

bool Exp_NumberIsZero(Number& number) {
    number.CorrectForSignificance();
    return number.GetDigits().size() == 1 && number.GetDigits()[0] == 0;
}

bool Exp_NumberIsSmallerThanOne(Number& number) {
    number.CorrectForSignificance();
    return number.GetExponent() < 0;
}



Number Exp_Average(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    a.CorrectForSignificance();
    b.CorrectForSignificance();

    // Dedicated fast path for the exponent code path (non-negative values).
    // Fallback keeps correctness for any future signed usage.
    if (a.GetIsNegative() || b.GetIsNegative()) {
        Addition addition;
        Number sum = addition.Calculate(a, b);
        bool wasNegative = sum.GetIsNegative();
        sum.SetNegative(false);

        result = HalfNonNegative(sum);
        result.SetNegative(wasNegative && !IsZeroFast(result));
        result.CorrectForSignificance();
        return result;
    }

    int topExp = std::max(a.GetExponent(), b.GetExponent());
    int aBottomExp = a.GetExponent() - static_cast<int>(a.GetDigits().size()) + 1;
    int bBottomExp = b.GetExponent() - static_cast<int>(b.GetDigits().size()) + 1;
    int bottomExp = std::min(aBottomExp, bBottomExp);

    auto digitAtExponent = [](Number& num, int exponent) {
        int index = num.GetExponent() - exponent;
        if (index < 0 || index >= static_cast<int>(num.GetDigits().size())) {
            return 0;
        }
        return num.GetDigits().at(index);
    };

    // Sum digits from least-significant exponent upward to propagate carry correctly.
    std::vector<int> sumDigitsLeastFirst;
    sumDigitsLeastFirst.reserve(topExp - bottomExp + 2);
    int carry = 0;
    for (int exponent = bottomExp; exponent <= topExp; exponent++) {
        int sumDigit = digitAtExponent(a, exponent) + digitAtExponent(b, exponent) + carry;
        sumDigitsLeastFirst.push_back(sumDigit % 10);
        carry = sumDigit / 10;
    }
    if (carry > 0) {
        sumDigitsLeastFirst.push_back(carry);
        topExp++;
    }

    // Long division by 2 on the summed number (most-significant digit first).
    std::vector<int> averageDigits;
    averageDigits.reserve(a.GetMaxSignificant());
    int remainder = 0;
    for (int i = static_cast<int>(sumDigitsLeastFirst.size()) - 1; i >= 0; i--) {
        int current = remainder * 10 + sumDigitsLeastFirst.at(i);
        averageDigits.push_back(current / 2);
        remainder = current % 2;
    }

    while (remainder != 0 && static_cast<int>(averageDigits.size()) < a.GetMaxSignificant()) {
        int current = remainder * 10;
        averageDigits.push_back(current / 2);
        remainder = current % 2;
    }

    result.SetNumber(false, averageDigits, topExp);
    result.CorrectForSignificance();
    return result;

}


Number Exp_SquareRoot(Number& number, int rootSignificant) {
    if (number.GetIsNegative()) {
        return Number(number.GetMaxSignificant()); // Return 0 if trying to calculate square root of negative number
    }
    if (IsZeroFast(number)) {
        return Number(number.GetMaxSignificant()); // Return 0 if trying to calculate square root of 0
    }

    // Newton-Raphson: x_{k+1} = (x_k + number / x_k) / 2
    Number current(rootSignificant);
    int sqrtExponent = number.GetExponent() >= 0 ? number.GetExponent() / 2 : (number.GetExponent() - 1) / 2;
    current.SetNumber(false, number.GetDigits(), sqrtExponent);
    current.CorrectForSignificance();

    Division division;
    int maxIterations = std::max(8, rootSignificant * 2 + 8);

    for (int i = 0; i < maxIterations; i++) {
        if (IsZeroFast(current)) {
            current.SetNumber(false, {1}, 0);
        }

        Number quotient = division.Calculate(number, current);
        Number next = Exp_Average(current, quotient);

        // Converged at current precision.
        if (CompareNumbers(next, current) == 0) {
            next.CorrectForSignificance();
            return next;
        }

        current = next;
    }

    current.CorrectForSignificance();
    return current;
}


Number Exp_Round(Number& number, int significant) {
    Number result(number.GetMaxSignificant());
    if (number.GetDigits().size() <= static_cast<size_t>(significant)) {
        result.SetNumber(number.GetIsNegative(), number.GetDigits(), number.GetExponent());
        return result;
    }

    std::vector<int> roundedDigits(number.GetDigits().begin(), number.GetDigits().begin() + significant);
    int nextDigit = number.GetDigits().at(significant);
    if (nextDigit >= 5) {
        // Round up
        for (int i = significant - 1; i >= 0; i--) {
            if (roundedDigits.at(i) < 9) {
                roundedDigits.at(i)++;
                break;
            } else {
                roundedDigits.at(i) = 0;
            }
        }
        if (roundedDigits.at(0) == 0) {
            // All digits were 9 and got rounded up to 0, so we need to add a new leading digit
            roundedDigits.insert(roundedDigits.begin(), 1);
            number.SetExponent(number.GetExponent() + 1);
        }
    }

    result.SetNumber(number.GetIsNegative(), roundedDigits, number.GetExponent());
    result.CorrectForSignificance();
    return result;
}

