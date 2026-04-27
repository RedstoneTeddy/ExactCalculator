#include "minmax.hpp"

#include "../calculation/base_structures.hpp"
#include "../calculation/number.hpp"
#include "../CalculationError.hpp"
#include "../calculation/compare.hpp"



Number Min::Calculate(Number& a, Number& b) {
    int comparison = CompareNumbers(a, b, false, 0);
    if (comparison <= 0) {
        return a;
    }
    return b;
}

Number Max::Calculate(Number& a, Number& b) {
    int comparison = CompareNumbers(a, b, false, 0);
    if (comparison >= 0) {
        return a;
    }
    return b;
}

