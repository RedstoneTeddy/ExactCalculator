#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"

#include "../CalculationError.hpp"


class RandomInt : public CalculationPart {
public:
    // Returns a random integer between 0 and n (inclusive)
    Number Calculate(Number n);
};



#endif // RANDOM_HPP