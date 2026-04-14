#ifndef MULTIPLICATION_HPP
#define MULTIPLICATION_HPP

#include "base_operation.hpp"
#include "number.hpp"
#include "subtraction.hpp"
#include "addition.hpp"

class Multiplication : public Subtraction {
public:
    Number Calculate(Number& a, Number& b);
};





#endif // MULTIPLICATION_HPP