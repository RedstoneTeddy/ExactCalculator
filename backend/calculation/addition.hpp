#ifndef ADDITION_HPP
#define ADDITION_HPP

#include "base_operation.hpp"
#include "compare.hpp"

class Addition : public Operation {
public:
    Number Calculate(Number& a, Number& b, int shiftB = 0) override;
};





#endif // ADDITION_HPP