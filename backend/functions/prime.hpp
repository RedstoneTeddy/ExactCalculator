#ifndef PRIME_HPP
#define PRIME_HPP


#include "../calculation/base_structures.hpp"

#include "../CalculationError.hpp"

#include "../calculation/number.hpp"
#include "../calculation/modulo.hpp"
#include "../calculation/division.hpp"
#include "../calculation/multiplication.hpp"

#include "combinatorics.hpp"

class Gcd : public CalculationPart {
public:
    Number Calculate(Number a, Number b);
};

class Lcm : public CalculationPart {
public:
    Number Calculate(Number a, Number b);
};

class Nthprime : public CalculationPart {
public:
    Number Calculate(Number n);
};


#endif // PRIME_HPP