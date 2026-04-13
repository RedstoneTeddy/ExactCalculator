#ifndef MULTIPLICATION_HPP
#define MULTIPLICATION_HPP

#include "base_operation.hpp"
#include "number.hpp"
#include "addition.hpp"

class Multiplication : public Addition {
public:
    Number Calculate(Number& a, Number& b) override;
};





#endif // MULTIPLICATION_HPP