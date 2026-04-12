#ifndef BASE_OPERATION_HPP
#define BASE_OPERATION_HPP

#include "base_structures.hpp"
#include "number.hpp"


class Operation : public CalculationPart {
public:
    virtual ~Operation() = default;
    virtual Number Calculate(Number& a, Number& b) = 0;
};



#endif // BASE_OPERATION_HPP