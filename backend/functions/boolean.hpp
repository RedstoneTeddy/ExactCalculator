#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP


#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/compare.hpp"


class BooleanEquals : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanNotEquals : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanGreater : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanGreaterEquals : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanLess : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanLessEquals : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanAnd : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanOr : public CalculationPart {
public:
    Number Calculate(Number left, Number right);
};

class BooleanNot : public CalculationPart {
public:
    Number Calculate(Number value);
};

class BooleanIf : public CalculationPart {
public:
    Number Calculate(Number condition, Number trueValue, Number falseValue);
};



#endif // BOOLEAN_HPP