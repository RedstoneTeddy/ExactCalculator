#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <string>
#include <vector>
#include "base_structures.hpp"
#include "number.hpp"

class Variable : public CalculationPart {
private:
    std::string name;

    std::vector<Number>* varNumbers;
    std::vector<std::string>* varNames;

public:
    Variable(std::string name, std::vector<Number>* varNumbers, std::vector<std::string>* varNames);

    void SetVectorPointers(std::vector<Number>* varNumbers, std::vector<std::string>* varNames) {
        this->varNumbers = varNumbers;
        this->varNames = varNames;
    }

    Number GetValue();
    std::string GetName() { return name; }
};


void SetVariable(std::string name, Number value, std::vector<Number>* varNumbers, std::vector<std::string>* varNames);


#endif // VARIABLES_HPP