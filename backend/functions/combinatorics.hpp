#ifndef COMBINATORICS_HPP
#define COMBINATORICS_HPP


#include "../calculation/base_structures.hpp"
#include "../calculation/number.hpp"
#include "factorial.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/subtraction.hpp"
#include "../calculation/division.hpp"


class nCr : public CalculationPart {
public:
    Number Calculate(Number n, Number r);
};

class nPr : public CalculationPart {
public:
    Number Calculate(Number n, Number r);
};


#endif // COMBINATORICS_HPP