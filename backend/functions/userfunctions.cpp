#include "userfunctions.hpp"


#include "../CalculationError.hpp"
#include "../clone_parts.hpp"



UserFunction::UserFunction(
    std::string name,
    std::vector<std::string>* allFunctionNames,
    std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas,
    std::vector<std::vector<std::string>>* allParameterNames 
) {
    this->name = std::move(name);
    this->allFunctionNames = allFunctionNames;
    this->allFormulas = allFormulas;
    this->allParameterNames = allParameterNames;
}

Number UserFunction::Calculate(
    std::vector<Number> arguments,
    Calc_main& calc_main
) {
    int formula_index = std::distance(allFunctionNames->begin(), std::find(allFunctionNames->begin(), allFunctionNames->end(), name));
    if (formula_index >= allFormulas->size()) {
        throw CalculationError("The function '" + name + "' is not defined.", ErrorType::VariableNotDefined);
    }

    std::vector<std::unique_ptr<CalculationPart>>& formula = (*allFormulas)[formula_index];
    std::vector<std::string>& parameterNames = (*allParameterNames)[formula_index];


    if (arguments.size() != parameterNames.size()) {
        throw CalculationError("The number of arguments provided does not match the number of parameters for the function '" + name + "'.", ErrorType::SyntaxError);
    }



    // Assign arguments to parameters
    for (int i = 0; i < parameterNames.size(); i++) {
        std::vector<std::unique_ptr<CalculationPart>> paramAssignment;
        paramAssignment.push_back(std::make_unique<Variable>(parameterNames[i], nullptr, nullptr));
        paramAssignment.push_back(std::make_unique<EqualSign>());
        paramAssignment.push_back(std::make_unique<Number>(arguments[i]));
        calc_main.Calculate_part(paramAssignment);
    }

    // Calculate formula
    std::vector<std::unique_ptr<CalculationPart>> formulaCopy = CloneCalculationParts(formula);
    return calc_main.Calculate_part(formulaCopy);
}


void DefineUserFunction(
    std::string name,
    std::vector<std::string> parameterNames,
    std::vector<std::unique_ptr<CalculationPart>> formula,
    std::vector<std::string>* allFunctionNames,
    std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas,
    std::vector<std::vector<std::string>>* allParameterNames
) {
    if (!allFunctionNames || !allFormulas || !allParameterNames) {
        throw CalculationError("Internal function storage is not initialized.", ErrorType::SyntaxError);
    }

    if (std::find(allFunctionNames->begin(), allFunctionNames->end(), name) != allFunctionNames->end()) {
        // Redefine existing function
        int index = std::distance(allFunctionNames->begin(), std::find(allFunctionNames->begin(), allFunctionNames->end(), name));
        (*allFormulas)[index] = CloneCalculationParts(formula);
        (*allFunctionNames)[index] = name;
        (*allParameterNames)[index] = parameterNames;
    } else {
        // Define new function
        allFunctionNames->push_back(name);
        allFormulas->push_back(CloneCalculationParts(formula));
        allParameterNames->push_back(parameterNames);
    }
};


