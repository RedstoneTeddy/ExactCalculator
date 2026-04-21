#include "variables.hpp"
#include "../CalculationError.hpp"

#include "number.hpp"
#include "base_structures.hpp"

#include <vector>
#include <string>
#include <iostream>

Variable::Variable(std::string name, std::vector<Number>* varNumbers, std::vector<std::string>* varNames) {
    this->name = std::move(name);
    this->varNumbers = varNumbers;
    this->varNames = varNames;
}

Number Variable::GetValue() {
    if (!varNames || !varNumbers) {
        throw CalculationError("Internal variable storage is not initialized.", ErrorType::SyntaxError);
    }
    for (int i = 0; i < varNames->size(); i++) {
        if ((*varNames)[i] == name) {
            return (*varNumbers)[i];
        }
    }
    throw CalculationError("The variable '" + name + "' is not defined.", ErrorType::VariableNotDefined);
}

void SetVariable(std::string name, Number value, std::vector<Number>* varNumbers, std::vector<std::string>* varNames) {
    if (!varNames || !varNumbers) {
        throw CalculationError("Internal variable storage is not initialized.", ErrorType::SyntaxError);
    }
    for (int i = 0; i < varNumbers->size(); i++) {
        if ((*varNames)[i] == name) {
            (*varNumbers)[i] = value;
            return;
        }
    }
    // If variable does not exist, add it
    varNames->push_back(name);
    varNumbers->push_back(value);
}