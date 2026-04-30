#include "modulo.hpp"

#include "number.hpp"
#include "../CalculationError.hpp"

#include "subtraction.hpp"
#include "multiplication.hpp"
#include "division.hpp"

#include "../functions/round.hpp"


Number Modulo::Calculate(Number& a, Number& b) {
    if (b.GetDigits().size() == 1 && b.GetDigits()[0] == 0) {
        throw CalculationError("Modulo by zero is not defined.", ErrorType::DivisionByZero);
    }

    Subtraction sub;
    Multiplication mult;
    Division div;
    Floor floor;

    Number divisionResult = div.Calculate(a, b);
    divisionResult = floor.Calculate(divisionResult);
    Number subtractionPart = mult.Calculate(b, divisionResult);

    Number result(a.GetMaxSignificant());
    result = sub.Calculate(a, subtractionPart);

    result.CorrectForSignificance();
    return result;
}