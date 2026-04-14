#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <vector>
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"


class Constant : public CalculationPart {
private:
    std::string name;
    int maxSignificant;
public:
    Constant(std::string name, int maxSignificant) {
        this->name = name;
        this->maxSignificant = maxSignificant;
    }
    std::string GetName() {return name;}

    Number GetValue();

};



#endif // CONSTANTS_HPP