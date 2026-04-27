#ifndef MINMAX_HPP
#define MINMAX_HPP


#include "../CalculationError.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"


class Min : public CalculationPart {
public:
    Number Calculate(Number& a, Number& b);
};

class Max : public CalculationPart {
public:
    Number Calculate(Number& a, Number& b);
};



#endif // MINMAX_HPP