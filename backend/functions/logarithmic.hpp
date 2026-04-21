#ifndef LOGARITHMIC_HPP
#define LOGARITHMIC_HPP

#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/exponent.hpp"


class Logarithm : public CalculationPart {
public:
    Number Calculate(Number base, Number input);
};

class NaturalLogarithm : public CalculationPart {
public:
    Number Calculate(Number input);
};



#endif