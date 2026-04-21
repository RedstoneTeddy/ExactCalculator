#include "trigonometric.hpp"

#include "../calculation/number.hpp"
#include "../calculation/compare.hpp"
#include "../calculation/addition.hpp"
#include "../calculation/subtraction.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/division.hpp"
#include "../calculation/exponent.hpp"
#include "constants.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace {
int GetGuardSignificance(int significance) {
    return std::max(significance + 20, significance);
}

bool IsZeroFast(Number& number) {
    return number.GetDigits().size() == 1 && number.GetDigits()[0] == 0;
}

Number MakeZero(int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString("0");
    return result;
}

Number MakeOne(int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString("1");
    return result;
}

Number MakeTwo(int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString("2");
    return result;
}

Number MakeHalf(int maxSignificant) {
    Division division;
    Number one = MakeOne(maxSignificant);
    Number two = MakeTwo(maxSignificant);
    return division.Calculate(one, two);
}

Number MakePi(int maxSignificant) {
    Constant piConstant("Pi", maxSignificant);
    return piConstant.GetValue();
}

Number MakeAbsCopy(Number value) {
    value.SetNegative(false);
    return value;
}

bool IsLessOrEqual(Number& left, Number& right) {
    return CompareNumbers(left, right) <= 0;
}

bool IsLess(Number& left, Number& right) {
    return CompareNumbers(left, right) < 0;
}

bool IsGreater(Number& left, Number& right) {
    return CompareNumbers(left, right) > 0;
}

Number ArcSineSeries(Number x) {
    int maxSignificant = x.GetMaxSignificant();

    Addition addition;
    Multiplication multiplication;
    Division division;

    Number sum = x;
    Number term = x;
    Number xSquared = multiplication.Calculate(x, x);

    int maxIterations = std::max(8, maxSignificant * 2 + 8);
    for (int n = 0; n < maxIterations; n++) {
        int numeratorInt = (2 * n + 1) * (2 * n + 1);
        int denominatorInt = (2 * n + 2) * (2 * n + 3);

        Number numerator(maxSignificant);
        Number denominator(maxSignificant);
        numerator.SetFromString(std::to_string(numeratorInt));
        denominator.SetFromString(std::to_string(denominatorInt));
        Number ratio = division.Calculate(numerator, denominator);

        term = multiplication.Calculate(term, xSquared);
        term = multiplication.Calculate(term, ratio);
        if (IsZeroFast(term)) {
            break;
        }

        Number next = addition.Calculate(sum, term);
        if (CompareNumbers(next, sum) == 0) {
            sum = next;
            break;
        }
        sum = next;
    }

    return sum;
}

Number ArcTangentSeries(Number x) {
    int maxSignificant = x.GetMaxSignificant();

    Addition addition;
    Multiplication multiplication;
    Division division;

    Number sum = x;
    Number term = x;
    Number minusXSquared = multiplication.Calculate(x, x);
    if (!IsZeroFast(minusXSquared)) {
        minusXSquared.SetNegative(true);
    }

    int maxIterations = std::max(8, maxSignificant * 2 + 8);
    for (int n = 0; n < maxIterations; n++) {
        Number numerator(maxSignificant);
        Number denominator(maxSignificant);
        numerator.SetFromString(std::to_string(2 * n + 1));
        denominator.SetFromString(std::to_string(2 * n + 3));
        Number ratio = division.Calculate(numerator, denominator);

        term = multiplication.Calculate(term, minusXSquared);
        term = multiplication.Calculate(term, ratio);
        if (IsZeroFast(term)) {
            break;
        }

        Number next = addition.Calculate(sum, term);
        if (CompareNumbers(next, sum) == 0) {
            sum = next;
            break;
        }
        sum = next;
    }

    return sum;
}

Number MakeInteger(int value, int maxSignificant) {
    Number result(maxSignificant);
    result.SetFromString(std::to_string(value));
    return result;
}

Number TruncateToIntegerPart(Number& value) {
    Number result(value.GetMaxSignificant());

    value.CorrectForSignificance();
    if (IsZeroFast(value) || value.GetExponent() < 0) {
        return result;
    }

    int integerDigits = value.GetExponent() + 1;
    const std::vector<int>& sourceDigits = value.GetDigits();
    std::vector<int> digits;
    digits.reserve(integerDigits);

    int copyCount = std::min(integerDigits, static_cast<int>(sourceDigits.size()));
    for (int i = 0; i < copyCount; i++) {
        digits.push_back(sourceDigits.at(i));
    }

    if (integerDigits > static_cast<int>(sourceDigits.size())) {
        digits.insert(digits.end(), integerDigits - static_cast<int>(sourceDigits.size()), 0);
    }

    result.SetNumber(false, digits, static_cast<int>(digits.size()) - 1);
    result.CorrectForSignificance();
    return result;
}

void ReduceIntoPeriod(Number& input, Number& period, Number& remainder) {
    Division division;
    Multiplication multiplication;
    Subtraction subtraction;
    Addition addition;

    Number quotient = division.Calculate(input, period);
    Number quotientInt = TruncateToIntegerPart(quotient);
    Number multiple = multiplication.Calculate(quotientInt, period);
    remainder = subtraction.Calculate(input, multiple);

    while (CompareNumbers(remainder, period) >= 0) {
        remainder = subtraction.Calculate(remainder, period);
    }

    Number zero(period.GetMaxSignificant());
    zero.SetFromString("0");
    while (CompareNumbers(remainder, zero) < 0) {
        remainder = addition.Calculate(remainder, period);
    }
}
} // namespace

Number Sine::Calculate(Number input) {
    if (input.GetIsNegative()) {
        input.SetNegative(false);
        Number result = Calculate(input);
        if (!IsZeroFast(result)) {
            result.SetNegative(!result.GetIsNegative());
        }
        return result;
    }

    int maxSignificant = input.GetMaxSignificant();
    Addition addition;
    Subtraction subtraction;
    Multiplication multiplication;
    Division division;

    Constant piConstant("Pi", maxSignificant);
    Number pi = piConstant.GetValue();

    Number two = MakeInteger(2, maxSignificant);
    Number period = multiplication.Calculate(pi, two);
    Number halfPeriod = division.Calculate(pi, two);

    Number reduced(input.GetMaxSignificant());
    ReduceIntoPeriod(input, period, reduced);

    bool isNegative = false;
    if (CompareNumbers(reduced, pi) > 0) {
        reduced = subtraction.Calculate(reduced, pi);
        isNegative = true;
    }

    if (CompareNumbers(reduced, halfPeriod) > 0) {
        reduced = subtraction.Calculate(pi, reduced);
    }

    if (IsZeroFast(reduced)) {
        return reduced;
    }

    Number reducedSquared = multiplication.Calculate(reduced, reduced);
    Number term = reduced;
    Number sum = reduced;

    int maxIterations = std::max(8, maxSignificant * 2 + 8);
    bool subtractNext = true;
    for (int i = 1; i <= maxIterations; i++) {
        Number numerator = multiplication.Calculate(term, reducedSquared);
        Number denominator = MakeInteger((2 * i) * (2 * i + 1), maxSignificant);

        term = division.Calculate(numerator, denominator);
        if (IsZeroFast(term)) {
            break;
        }

        Number nextSum = subtractNext ? subtraction.Calculate(sum, term) : addition.Calculate(sum, term);
        if (CompareNumbers(nextSum, sum) == 0) {
            sum = nextSum;
            break;
        }

        sum = nextSum;
        subtractNext = !subtractNext;
    }

    if (isNegative && !IsZeroFast(sum)) {
        sum.SetNegative(true);
    }

    sum.SetMaxSignificant(input.GetMaxSignificant());
    sum = Exp_Round(sum, input.GetMaxSignificant());
    sum.CorrectForSignificance();
    return sum;
}

Number Cosine::Calculate(Number input) {
    int maxSignificant = input.GetMaxSignificant();

    Constant piConstant("Pi", maxSignificant);
    Number pi = piConstant.GetValue();

    Multiplication multiplication;
    Division division;
    Subtraction subtraction;

    Number two = MakeInteger(2, maxSignificant);
    Number halfPi = division.Calculate(pi, two);

    Number shifted = subtraction.Calculate(halfPi, input);

    Sine sine;
    Number result = sine.Calculate(shifted);
    return result;
}

Number Tangent::Calculate(Number input) {
    int maxSignificant = input.GetMaxSignificant();

    Sine sine;
    Cosine cosine;
    Division division;

    Number sineValue = sine.Calculate(input);
    Number cosineValue = cosine.Calculate(input);

    if (IsZeroFast(cosineValue)) {
        return Number(maxSignificant);
    }

    Number result = division.Calculate(sineValue, cosineValue);
    result.SetMaxSignificant(maxSignificant);
    result = Exp_Round(result, input.GetMaxSignificant());
    result.CorrectForSignificance();

    return result;
}

Number ArcSine::Calculate(Number input) {
    int workingSignificant = GetGuardSignificance(input.GetMaxSignificant());

    Number x = input;
    x.SetMaxSignificant(workingSignificant);
    x.CorrectForSignificance();

    Number zero = MakeZero(workingSignificant);
    Number one = MakeOne(workingSignificant);
    Number two = MakeTwo(workingSignificant);
    Number half = MakeHalf(workingSignificant);
    Number pi = MakePi(workingSignificant);
    Number halfPi = Division().Calculate(pi, two);

    Number absX = MakeAbsCopy(x);
    if (IsGreater(absX, one)) {
        return zero;
    }

    if (x.GetIsNegative()) {
        x.SetNegative(false);
        Number result = Calculate(x);
        result.SetNegative(!result.GetIsNegative());
        return result;
    }

    if (CompareNumbers(x, one) == 0) {
        return halfPi;
    }

    if (IsGreater(x, half)) {
        // asin(x) = pi/2 - 2*asin(sqrt((1-x)/2)) for x in (0.5, 1)
        Subtraction subtraction;
        Multiplication multiplication;
        Number oneMinusX = subtraction.Calculate(one, x);
        Number reduced = Division().Calculate(oneMinusX, two);
        Number inner = Exp_SquareRoot(reduced, workingSignificant);
        Number innerAsin = Calculate(inner);
        Number doubleInner = multiplication.Calculate(two, innerAsin);
        return subtraction.Calculate(halfPi, doubleInner);
    }

    Number result = ArcSineSeries(x);
    result.SetMaxSignificant(input.GetMaxSignificant());
    result = Exp_Round(result, input.GetMaxSignificant());
    result.CorrectForSignificance();
    return result;
}

Number ArcCosine::Calculate(Number input) {
    int workingSignificant = GetGuardSignificance(input.GetMaxSignificant());

    Number x = input;
    x.SetMaxSignificant(workingSignificant);
    x.CorrectForSignificance();

    Number zero = MakeZero(workingSignificant);
    Number one = MakeOne(workingSignificant);
    Number two = MakeTwo(workingSignificant);
    Number pi = MakePi(workingSignificant);
    Number halfPi = Division().Calculate(pi, two);
    Subtraction subtraction;
    Addition addition;
    Multiplication multiplication;
    Division division;

    Number absX = MakeAbsCopy(x);
    if (IsGreater(absX, one)) {
        return zero;
    }

    if (CompareNumbers(x, one) == 0) {
        return zero;
    }
    if (CompareNumbers(x, zero) == 0) {
        return halfPi;
    }

    Number half = division.Calculate(one, two);
    Number absXCenter = MakeAbsCopy(x);
    if (IsLessOrEqual(absXCenter, half)) {
        // Fast, stable in the center interval.
        Number asinX = ArcSine().Calculate(x);
        return subtraction.Calculate(halfPi, asinX);
    }

    if (x.GetIsNegative()) {
        // acos(x) = pi - 2*asin(sqrt((1+x)/2)) for x in [-1, 0)
        Number onePlusX = addition.Calculate(one, x);
        Number reduced = Division().Calculate(onePlusX, two);
        Number inner = Exp_SquareRoot(reduced, workingSignificant);
        Number innerAsin = ArcSine().Calculate(inner);
        Number doubleInner = Multiplication().Calculate(two, innerAsin);
        return subtraction.Calculate(pi, doubleInner);
    }

    // acos(x) = 2*asin(sqrt((1-x)/2)) for x in (0, 1)
    Number oneMinusX = subtraction.Calculate(one, x);
    Number reduced = Division().Calculate(oneMinusX, two);
    Number inner = Exp_SquareRoot(reduced, workingSignificant);
    Number innerAsin = ArcSine().Calculate(inner);

    Number result = multiplication.Calculate(two, innerAsin);
    result.SetMaxSignificant(input.GetMaxSignificant());
    result = Exp_Round(result, input.GetMaxSignificant());
    result.CorrectForSignificance();

    return result;
}

Number ArcTangent::Calculate(Number input) {
    int workingSignificant = GetGuardSignificance(input.GetMaxSignificant());

    Number x = input;
    x.SetMaxSignificant(workingSignificant);
    x.CorrectForSignificance();

    Number zero = MakeZero(workingSignificant);
    Number one = MakeOne(workingSignificant);
    Number two = MakeTwo(workingSignificant);
    Number pi = MakePi(workingSignificant);
    Division division;
    Number halfPi = division.Calculate(pi, two);

    if (IsZeroFast(x)) {
        return zero;
    }

    if (x.GetIsNegative()) {
        x.SetNegative(false);
        Number result = Calculate(x);
        result.SetNegative(!result.GetIsNegative());
        return result;
    }

    if (IsGreater(x, one)) {
        // atan(x) = pi/2 - atan(1/x) for x > 1
        Number reciprocal = division.Calculate(one, x);
        Number inner = Calculate(reciprocal);
        Subtraction subtraction;
        return subtraction.Calculate(halfPi, inner);
    }

    if (CompareNumbers(x, one) == 0) {
        Number quarterPi = division.Calculate(halfPi, two);
        return quarterPi;
    }

    // For moderate inputs, one reduction step makes the Newton solve much faster.
    Number half = division.Calculate(one, two);
    if (IsGreater(x, half)) {
        Multiplication multiplication;
        Addition addition;
        Number xSquared = multiplication.Calculate(x, x);
        Number onePlusXSquared = addition.Calculate(one, xSquared);
        Number sqrtPart = Exp_SquareRoot(onePlusXSquared, workingSignificant);
        Number reducedDenominator = addition.Calculate(one, sqrtPart);
        Number reduced = division.Calculate(x, reducedDenominator);
        Number inner = Calculate(reduced);
        return multiplication.Calculate(two, inner);
    }

    Number result = ArcTangentSeries(x);
    result.SetMaxSignificant(input.GetMaxSignificant());
    result = Exp_Round(result, input.GetMaxSignificant());
    result.CorrectForSignificance();

    return result;
}
