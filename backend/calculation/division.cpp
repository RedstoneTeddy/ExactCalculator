#include "division.hpp"
#include "base_operation.hpp"
#include "number.hpp"
#include "multiplication.hpp"
#include "compare.hpp"
#include "../CalculationError.hpp"

#include <algorithm>
#include <vector>

namespace {
bool IsZeroFast(Number& number) {
    return number.GetDigits().size() == 1 && number.GetDigits()[0] == 0;
}

int CompareShiftedNonNegative(Number& left, Number& right, int shiftRight) {
    // Compares left with (right * 10^shiftRight), assuming both are non-negative and normalized.
    if (IsZeroFast(left) && IsZeroFast(right)) {
        return 0;
    }
    if (IsZeroFast(left)) {
        return -1;
    }
    if (IsZeroFast(right)) {
        return 1;
    }

    int rightExponent = right.GetExponent() + shiftRight;
    if (left.GetExponent() > rightExponent) {
        return 1;
    }
    if (left.GetExponent() < rightExponent) {
        return -1;
    }

    int maxDigits = std::max(static_cast<int>(left.GetDigits().size()), static_cast<int>(right.GetDigits().size()));
    for (int i = 0; i < maxDigits; i++) {
        int leftDigit = i < static_cast<int>(left.GetDigits().size()) ? left.GetDigits()[i] : 0;
        int rightDigit = i < static_cast<int>(right.GetDigits().size()) ? right.GetDigits()[i] : 0;

        if (leftDigit > rightDigit) {
            return 1;
        }
        if (leftDigit < rightDigit) {
            return -1;
        }
    }

    return 0;
}

int FindQuotientDigit(Number& left, std::vector<Number>& divisors, int shift) {
    int low = 1;
    int high = 9;
    int best = 0;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = CompareShiftedNonNegative(left, divisors[mid - 1], shift);
        if (cmp >= 0) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return best;
}
} // namespace


Number Division::Calculate(Number& a, Number& b) {
    if (a.GetDigits().empty()) {
        throw CalculationError("Division operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }
    if (b.GetDigits().empty()) {
        throw CalculationError("Division operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }

    a.CorrectForSignificance();
    b.CorrectForSignificance();

    if (IsZeroFast(b)) {
        throw CalculationError("Division by zero is not allowed.", ErrorType::DivisionByZero);
    }

    Number result(a.GetMaxSignificant());

    Number a_leftover(a.GetMaxSignificant());
    a_leftover.SetNumber(false, a.GetDigits(), a.GetExponent());

    Number posB(a.GetMaxSignificant());
    posB.SetNumber(false, b.GetDigits(), b.GetExponent());

    std::vector<Number> divisors;
    divisors.reserve(9);
    divisors.push_back(posB);
    for (int i = 1; i < 9; i++) {
        Number nextDivisor = Addition::Calculate(divisors[i - 1], posB);
        divisors.push_back(nextDivisor);
    }
    
    // Do the division
    int i_limit = result.GetMaxSignificant() + 1;
    result.GetDigits().reserve(i_limit + 1);
    int baseShift = a.GetExponent() - b.GetExponent();

    for (int i = 0; i <= i_limit; i++) {
        int shift = baseShift - i;
        int resultDigit = FindQuotientDigit(a_leftover, divisors, shift);

        if (resultDigit > 0) {
            a_leftover = Subtraction::Calculate(a_leftover, divisors[resultDigit - 1], shift);
        }
        result.GetDigits().push_back(resultDigit);

        if (IsZeroFast(a_leftover)) {
            break;
        }

    }


    result.SetExponent(a.GetExponent() - b.GetExponent() + 1);
    result.SetNegative(a.GetIsNegative() != b.GetIsNegative());
    result.CorrectForSignificance();
    return result;
}