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
    bool isFunctionBracket;
    Bracket(bool isOpen, bool isFunctionBracket) {
        this->isOpen = isOpen;
        this->isFunctionBracket = isFunctionBracket;
    }

};

class CommaSeparator : public CalculationPart {
public:    
    CommaSeparator() = default;
};




#endif // BASE_STRUCTURES_HPP