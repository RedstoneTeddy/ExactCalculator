#ifndef SUBTRACTION_HPP
#define SUBTRACTION_HPP

#include "base_operation.hpp"

class Subtraction : public Operation {
public:
    Number Calculate(Number& a, Number& b) override;
};





#endif // SUBTRACTION_HPP