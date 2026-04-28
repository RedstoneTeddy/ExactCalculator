#include "combinatorics.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/number.hpp"
#include "factorial.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/subtraction.hpp"
#include "../calculation/division.hpp"


Number nPr::Calculate(Number n, Number r) {
    Factorial factorial;
    Multiplication multiplication;
    Subtraction subtraction;
    Division division;

    Number nFactorial = factorial.Calculate(n);
    Number nMinusR = subtraction.Calculate(n, r);
    Number nMinusRFactorial = factorial.Calculate(nMinusR);

    return division.Calculate(nFactorial, nMinusRFactorial);
}


Number nCr::Calculate(Number n, Number r) {
    Factorial factorial;
    Multiplication multiplication;
    Subtraction subtraction;
    Division division;

    Number nFactorial = factorial.Calculate(n);
    Number rFactorial = factorial.Calculate(r);
    Number nMinusR = subtraction.Calculate(n, r);
    Number nMinusRFactorial = factorial.Calculate(nMinusR);

    Number denominator = multiplication.Calculate(rFactorial, nMinusRFactorial);
    return division.Calculate(nFactorial, denominator);
}



