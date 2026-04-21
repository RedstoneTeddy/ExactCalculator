#include "factorial.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/compare.hpp"

#include <iostream>

Number Factorial::Calculate(Number input) {
    if (input.GetIsNegative()) {
        return Number(0);
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


