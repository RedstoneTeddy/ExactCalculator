#ifndef TRIGONOMETRIC_HPP
#define TRIGONOMETRIC_HPP

#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"

class Sine : public CalculationPart {
public:
    Number Calculate(Number input);
};

class Cosine : public CalculationPart {
public:
    Number Calculate(Number input);
};

class Tangent : public CalculationPart {
public:
    Number Calculate(Number input);
};

class ArcSine : public CalculationPart {
public:
    Number Calculate(Number input);
};

class ArcCosine : public CalculationPart {
public:
    Number Calculate(Number input);
};

class ArcTangent : public CalculationPart {
public:
    Number Calculate(Number input);
};




#endif // TRIGONOMETRIC_HPP