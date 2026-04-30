#ifndef MODULO_HPP
#define MODULO_HPP

#include "base_structures.hpp"
#include "base_operation.hpp"
#include "number.hpp"

class Modulo : public Operation {
public:
    Number Calculate(Number& a, Number& b) override;
};




#endif // MODULO_HPP