#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP

#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/multiplication.hpp"

class Factorial : public CalculationPart {
public:
    Number Calculate(Number input);
};

#endif // FACTORIAL_HPP