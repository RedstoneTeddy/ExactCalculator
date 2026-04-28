#ifndef ROUND_HPP
#define ROUND_HPP



#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/addition.hpp"


class Round : public CalculationPart {
public:
    Number Calculate(Number number);
};

class Ceil : public CalculationPart {
public:
    Number Calculate(Number number);
};

class Floor : public CalculationPart {
public:
    Number Calculate(Number number);
};




#endif // ROUND_HPP