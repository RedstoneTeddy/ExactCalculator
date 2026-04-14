#include "variables.hpp"

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
        std::cout << "Error: Variable pointers not initialized. Returning 0." << std::endl;
        return Number(0);
    }
    for (int i = 0; i < varNames->size(); i++) {
        if ((*varNames)[i] == name) {
            return (*varNumbers)[i];
        }
    }
    // If variable not found, return a default value (e.g., 0)

    std::cout << "Error: Variable '" << name << "' not found. Returning 0." << std::endl;
    return Number(0);
}

void SetVariable(std::string name, Number value, std::vector<Number>* varNumbers, std::vector<std::string>* varNames) {
    if (!varNames || !varNumbers) {
        std::cout << "Error: Variable pointers not initialized." << std::endl;
        return;
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