#include "boolean.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/compare.hpp"


bool CheckNumberIsTrue(Number number) {
    // A number is considered "true" if it is not equal to zero.
    Number NumZero(number.GetMaxSignificant());
    NumZero.SetDigits({0});
    return CompareNumbers(number, NumZero) != 0;
}



Number BooleanEquals::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CompareNumbers(left, right) == 0) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanNotEquals::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CompareNumbers(left, right) != 0) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanGreater::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CompareNumbers(left, right) > 0) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanGreaterEquals::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CompareNumbers(left, right) >= 0) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanLess::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CompareNumbers(left, right) < 0) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanLessEquals::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CompareNumbers(left, right) <= 0) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanAnd::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CheckNumberIsTrue(left) && CheckNumberIsTrue(right)) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanOr::Calculate(Number left, Number right) {
    Number result(left.GetMaxSignificant());

    if (CheckNumberIsTrue(left) || CheckNumberIsTrue(right)) {
        result.SetDigits({1});
    } else {
        result.SetDigits({0});
    }

    return result;
}

Number BooleanNot::Calculate(Number value) {
    Number result(value.GetMaxSignificant());

    if (CheckNumberIsTrue(value)) {
        result.SetDigits({0});
    } else {
        result.SetDigits({1});
    }

    return result;
}

Number BooleanIf::Calculate(Number condition, Number trueValue, Number falseValue) {
    Number result(condition.GetMaxSignificant());

    if (CheckNumberIsTrue(condition)) {
        result.SetDigits(trueValue.GetDigits());
        result.SetExponent(trueValue.GetExponent());
        result.SetNegative(trueValue.GetIsNegative());
    } else {
        result.SetDigits(falseValue.GetDigits());
        result.SetExponent(falseValue.GetExponent());
        result.SetNegative(falseValue.GetIsNegative());
    }

    return result;
}

