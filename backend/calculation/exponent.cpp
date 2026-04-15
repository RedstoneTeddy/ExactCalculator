#include "Exponent.hpp"
#include "division.hpp"
#include "addition.hpp"
#include "number.hpp"
#include "compare.hpp"
#include <algorithm>
#include <vector>

Number Exponent::Calculate(Number& a, Number& b) {
    return Calculate(a, b, a.GetMaxSignificant()/2);
}




Number Exponent::Calculate(Number& a, Number& b, int rootSignificant) {
    Number result(a.GetMaxSignificant());
    result.SetNumber(false, {1}, 0); // Start with 1
    
    Number one(a.GetMaxSignificant());
    one.SetNumber(false, {1}, 0);

    Number b_leftover(rootSignificant);
    b_leftover.SetNumber(false, b.GetDigits(), b.GetExponent());
    b_leftover.SetMaxSignificant(rootSignificant);
    b_leftover.CorrectForSignificance();


    // Base exponent (before comma)
    while (!NumberIsZero(b_leftover) && !NumberIsSmallerThanOne(b_leftover)) {
        result = Multiplication::Calculate(result, a);
        b_leftover = Subtraction::Calculate(b_leftover, one);
    }


    // Root-exponent (after comma)
    if (!NumberIsZero(b_leftover)) {
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
            current_divider = Average(current_divider, numZero);
            Number new_root = SquareRoot(current_root, rootSignificant);

            if (CompareNumbers(b_leftover, current_divider) >= 0) {
                result = Multiplication::Calculate(result, new_root);
                b_leftover = Subtraction::Calculate(b_leftover, current_divider);
            }

            // Check if calculation is finished
            run = true;
            b_leftover.CorrectForSignificance();
            if (NumberIsZero(b_leftover)) {
                run = false;
            }

            current_divider.CorrectForSignificance();
            if (NumberIsZero(current_divider)) {
                run = false;
            }

            current_root.CorrectForSignificance();
            new_root.CorrectForSignificance();
            if (CompareNumbers(new_root, current_root) == 0) {
                run = false;
            }
            current_root.SetDigits(new_root.GetDigits());
            current_root.SetExponent(new_root.GetExponent());

            i++;
            if (i > max_i) {
                run = false;
            }

        }




    }




    // Inverse of result if exponent is negative
    if (b.GetIsNegative()) {
        if (NumberIsZero(result)) {
            return Number(a.GetMaxSignificant()); // Return 0 if trying to calculate inverse of 0
        }
        result = Division::Calculate(one, result);
    }
    result = Round(result, rootSignificant-2);
    return result;
}




bool NumberHasComma(Number& number) {
    number.CorrectForSignificance();
    return 0 > number.GetExponent() - number.GetDigits().size() + 1;
}

bool NumberIsZero(Number& number) {
    number.CorrectForSignificance();
    return number.GetDigits().size() == 1 && number.GetDigits()[0] == 0;
}

bool NumberIsSmallerThanOne(Number& number) {
    number.CorrectForSignificance();
    return number.GetExponent() < 0;
}



Number Average(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    a.CorrectForSignificance();
    b.CorrectForSignificance();

    // Dedicated fast path for the exponent code path (non-negative values).
    // Fallback keeps correctness for any future signed usage.
    if (a.GetIsNegative() || b.GetIsNegative()) {
        Addition addition;
        Number sum = addition.Calculate(a, b);
        Number two(a.GetMaxSignificant());
        two.SetNumber(false, {2}, 0);
        Division division;
        result = division.Calculate(sum, two);
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


Number SquareRoot(Number& number, int rootSignificant) {
    if (number.GetIsNegative()) {
        return Number(number.GetMaxSignificant()); // Return 0 if trying to calculate square root of negative number
    }
    if (NumberIsZero(number)) {
        return Number(number.GetMaxSignificant()); // Return 0 if trying to calculate square root of 0
    }

    // Newton-Raphson: x_{k+1} = (x_k + number / x_k) / 2
    Number current(rootSignificant);
    if (NumberIsSmallerThanOne(number)) {
        current.SetNumber(false, {1}, 0);
    } else {
        current.SetNumber(false, number.GetDigits(), number.GetExponent());
    }
    current.CorrectForSignificance();

    Division division;
    int maxIterations = std::max(8, rootSignificant * 2 + 8);

    for (int i = 0; i < maxIterations; i++) {
        if (NumberIsZero(current)) {
            current.SetNumber(false, {1}, 0);
        }

        Number quotient = division.Calculate(number, current);
        Number next = Average(current, quotient);

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


Number Round(Number& number, int significant) {
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
        }
    }

    result.SetNumber(number.GetIsNegative(), roundedDigits, number.GetExponent());
    result.CorrectForSignificance();
    return result;
}

