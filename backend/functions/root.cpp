#include "root.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/exponent.hpp"
#include "../calculation/division.hpp"
#include "../calculation/compare.hpp"
#include "../CalculationError.hpp"


Number SquareRoot::Calculate(Number input) {
    Number r(input.GetMaxSignificant());
    r.SetFromString("0.5");

    Exponent exp(input.GetMaxSignificant()/2);

    return exp.Calculate(input, r);
}

Number Root::Calculate(Number degree, Number input) {
    Division div;
    Exponent exp(input.GetMaxSignificant()/2);

    Number zero(input.GetMaxSignificant());
    Number one(input.GetMaxSignificant());
    zero.SetFromString("0");
    one.SetFromString("1");

    if (CompareNumbers(degree, zero) == 0) {
        throw CalculationError("The root degree cannot be zero because it would require division by zero.", ErrorType::DivisionByZero);
    }

    Number r = div.Calculate(one, degree);
    
    return exp.Calculate(input, r);
};


