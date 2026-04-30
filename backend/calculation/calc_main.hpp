#ifndef CALC_MAIN_HPP
#define CALC_MAIN_HPP

#include <algorithm>
#include <memory>
#include <utility>
#include <iostream>
#include "addition.hpp"
#include "subtraction.hpp"
#include "multiplication.hpp"
#include "division.hpp"
#include "exponent.hpp"
#include "variables.hpp"
#include "equal_sign.hpp"

#include "../functions/constants.hpp"
#include "../functions/factorial.hpp"




class Calc_main {
private:
    std::vector<Number> varNumbers;
    std::vector<std::string> varNames;
    
    std::vector<std::string>* allFunctionNames;
    std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas;
    std::vector<std::vector<std::string>>* allParameterNames;


public:
    std::vector<Number> extractNumbers(int i, int openFunctionBracket, std::vector<std::unique_ptr<CalculationPart>>& calculation_parts);
    std::vector<std::vector<std::unique_ptr<CalculationPart>>> extractSubCalculations(int i, int openFunctionBracket, std::vector<std::unique_ptr<CalculationPart>>& calculation_parts);

    Number Calculate_part(std::vector<std::unique_ptr<CalculationPart>>& calculation_parts);
    Calc_main();
    ~Calc_main();
    
    std::vector<std::string> GetVariableNames() {
        return varNames;
    }

    void SetVariable(std::string name, Number value) {
        auto it = std::find(varNames.begin(), varNames.end(), name);
        if (it != varNames.end()) {
            size_t index = std::distance(varNames.begin(), it);
            varNumbers[index] = value;
        }
        else {
            varNames.push_back(name);
            varNumbers.push_back(value);
        }
    }

    void DeleteVariable(std::string name) {
        auto it = std::find(varNames.begin(), varNames.end(), name);
        if (it != varNames.end()) {
            size_t index = std::distance(varNames.begin(), it);
            varNames.erase(it);
            varNumbers.erase(varNumbers.begin() + index);
        }
    }
};


#endif // CALC_MAIN_HPP
