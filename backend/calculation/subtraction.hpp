#ifndef SUBTRACTION_HPP
#define SUBTRACTION_HPP

#include "base_operation.hpp"
#include "addition.hpp"
#include <iostream>

class Subtraction : public Addition {
public:
    Number Calculate(Number& a, Number& b) override;
    Number Calculate(Number& a, Number& b, int shiftB);
};





#endif // SUBTRACTION_HPP