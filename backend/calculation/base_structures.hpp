#ifndef BASE_STRUCTURES_HPP
#define BASE_STRUCTURES_HPP

// #include "number.hpp"


// On this class depend: Number, Operation, Bracket, Function
class CalculationPart {
public:
    virtual ~CalculationPart() = default;
};


class Bracket : public CalculationPart {
public:
    bool isOpen; // true for '(', false for ')'
    Bracket(bool isOpen) {this->isOpen = isOpen;}
};




#endif // BASE_STRUCTURES_HPP