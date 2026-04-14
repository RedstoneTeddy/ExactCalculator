#ifndef DIVISION_HPP
#define DIVISION_HPP

#include "base_operation.hpp"
#include "number.hpp"
#include "multiplication.hpp"
#include "compare.hpp"

class Division : public Multiplication {
public:
    Number Calculate(Number& a, Number& b);
};



#endif // DIVISION_HPP