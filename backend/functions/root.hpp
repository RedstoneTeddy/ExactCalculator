#ifndef ROOT_HPP
#define ROOT_HPP

#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/exponent.hpp"


class SquareRoot : public CalculationPart {
public:
    Number Calculate(Number input);
};

class Root : public CalculationPart {
public:
    Number Calculate(Number degree, Number input);
};

#endif // ROOT_HPP