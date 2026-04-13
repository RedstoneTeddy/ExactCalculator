#ifndef SUBTRACTION_HPP
#define SUBTRACTION_HPP

#include "base_operation.hpp"
#include "addition.hpp"

class Subtraction : public Addition {
public:
    Number Calculate(Number& a, Number& b, int shiftB = 0) override;
};





#endif // SUBTRACTION_HPP