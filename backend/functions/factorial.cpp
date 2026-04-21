#include "factorial.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/compare.hpp"
#include "../CalculationError.hpp"

#include <iostream>

Number Factorial::Calculate(Number input) {
    input.CorrectForSignificance();

    if (input.GetIsNegative()) {
        throw CalculationError("Factorial is only defined for non-negative integers.", ErrorType::DomainError);
    }

    int bottomExponent = input.GetExponent() - static_cast<int>(input.GetDigits().size()) + 1;
    if (bottomExponent < 0) {
        throw CalculationError("Factorial is only defined for whole numbers.", ErrorType::DomainError);
    }


    Number result(input.GetMaxSignificant());
    result.SetFromString("1");

    Number i(input.GetMaxSignificant());
    i.SetFromString("1");

    Number one(input.GetMaxSignificant());
    one.SetFromString("1");

    Multiplication mult;
    Addition add;

    while (CompareNumbers(i, input) <= 0) {
        result = mult.Calculate(result, i);
        i = add.Calculate(i, one);
    }

    return result;
}


