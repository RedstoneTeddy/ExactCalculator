#include "logarithmic.hpp"

#include "../calculation/addition.hpp"
#include "../calculation/subtraction.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/division.hpp"
#include "../calculation/compare.hpp"

#include <algorithm>
#include <string>

namespace {
bool IsZeroFast(Number& number) {
    return number.GetDigits().size() == 1 && number.GetDigits()[0] == 0;
}

Number MakeInteger(int value, int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString(std::to_string(value));
    return result;
}

Number MakeOne(int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString("1");
    return result;
}

Number MakeZero(int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString("0");
    return result;
}

Number MakeLn10(int maxSignificant) {
    Number ln10(maxSignificant);
    ln10.SetFromString("2.302585092994045684017991454684364207601101488628772976033327900967572609677352480235997205089598298");
    return ln10;
}

bool IsLessOrEqual(Number& left, Number& right) {
    return CompareNumbers(left, right) <= 0;
}

bool IsGreater(Number& left, Number& right) {
    return CompareNumbers(left, right) > 0;
}
} // namespace

Number Logarithm::Calculate(Number base, Number input) {
    int finalSignificant = std::max(base.GetMaxSignificant(), input.GetMaxSignificant());
    int workingSignificant = std::max(finalSignificant + 20, finalSignificant);

    base.SetMaxSignificant(workingSignificant);
    input.SetMaxSignificant(workingSignificant);
    base.CorrectForSignificance();
    input.CorrectForSignificance();

    Number zero = MakeZero(workingSignificant);
    Number one = MakeOne(workingSignificant);

    // Domain checks for real logarithm.
    if (IsLessOrEqual(input, zero) || IsLessOrEqual(base, zero) || CompareNumbers(base, one) == 0) {
        return MakeZero(finalSignificant);
    }

    NaturalLogarithm naturalLog;
    Number lnInput = naturalLog.Calculate(input);
    Number lnBase = naturalLog.Calculate(base);

    if (IsZeroFast(lnBase)) {
        return MakeZero(finalSignificant);
    }

    Division division;
    Number result = division.Calculate(lnInput, lnBase);
    return Exp_Round(result, finalSignificant);
}

Number NaturalLogarithm::Calculate(Number input) {
    int finalSignificant = input.GetMaxSignificant();
    int workingSignificant = std::max(finalSignificant + 20, finalSignificant);

    Number x = input;
    x.SetMaxSignificant(workingSignificant);
    x.CorrectForSignificance();

    Number zero = MakeZero(workingSignificant);
    Number one = MakeOne(workingSignificant);

    if (IsLessOrEqual(x, zero)) {
        return MakeZero(finalSignificant);
    }
    if (CompareNumbers(x, one) == 0) {
        return MakeZero(finalSignificant);
    }

    Addition addition;
    Subtraction subtraction;
    Multiplication multiplication;
    Division division;

    bool wasSmallerThanOne = false;
    if (!IsGreater(x, one)) {
        x = division.Calculate(one, x);
        wasSmallerThanOne = true;
    }

    // Decompose x = m * 10^e with m in [1, 10).
    x.CorrectForSignificance();
    int exponent10 = x.GetExponent();

    Number mantissa(workingSignificant);
    mantissa.SetNumber(false, x.GetDigits(), 0);
    mantissa.CorrectForSignificance();

    // Bring mantissa near 1 by repeated square roots.
    Number targetUpper(workingSignificant);
    targetUpper.SetFromString("1.1");
    int sqrtReductionSteps = 0;
    int maxSqrtSteps = 12;
    while (IsGreater(mantissa, targetUpper) && sqrtReductionSteps < maxSqrtSteps) {
        mantissa = Exp_SquareRoot(mantissa, workingSignificant);
        sqrtReductionSteps++;
    }

    // ln(m) = 2 * (y + y^3/3 + y^5/5 + ...), y = (m-1)/(m+1)
    Number mantissaMinusOne = subtraction.Calculate(mantissa, one);
    Number mantissaPlusOne = addition.Calculate(mantissa, one);
    Number y = division.Calculate(mantissaMinusOne, mantissaPlusOne);
    Number ySquared = multiplication.Calculate(y, y);

    Number term = y;
    Number seriesSum = y;

    int maxIterations = std::max(10, workingSignificant * 2 + 8);
    for (int n = 1; n < maxIterations; n++) {
        term = multiplication.Calculate(term, ySquared);
        if (IsZeroFast(term)) {
            break;
        }

        Number denominator = MakeInteger(2 * n + 1, workingSignificant);
        Number addTerm = division.Calculate(term, denominator);
        Number next = addition.Calculate(seriesSum, addTerm);

        if (CompareNumbers(next, seriesSum) == 0) {
            seriesSum = next;
            break;
        }
        seriesSum = next;
    }

    Number two = MakeInteger(2, workingSignificant);
    Number lnMantissaReduced = multiplication.Calculate(two, seriesSum);

    if (sqrtReductionSteps > 0) {
        Number scale = MakeInteger(1 << sqrtReductionSteps, workingSignificant);
        lnMantissaReduced = multiplication.Calculate(lnMantissaReduced, scale);
    }

    Number ln10 = MakeLn10(workingSignificant);
    Number exponentValue = MakeInteger(exponent10, workingSignificant);
    Number exponentPart = multiplication.Calculate(exponentValue, ln10);

    Number result = addition.Calculate(lnMantissaReduced, exponentPart);
    if (wasSmallerThanOne && !IsZeroFast(result)) {
        result.SetNegative(!result.GetIsNegative());
    }

    return Exp_Round(result, finalSignificant);
}