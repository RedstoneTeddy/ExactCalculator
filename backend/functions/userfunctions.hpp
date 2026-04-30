#ifndef USERFUNCTIONS_HPP
#define USERFUNCTIONS_HPP


#include "../CalculationError.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/variables.hpp"
#include "../calculation/number.hpp"
#include "../calculation/calc_main.hpp"
#include "../clone_parts.hpp"

#include <vector>
#include <memory>


class UserFunction : public CalculationPart {
private:
    std::string name;

    std::vector<std::string>* allFunctionNames;
    std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas;
    std::vector<std::vector<std::string>>* allParameterNames;

public:
    UserFunction(
        std::string name,
        std::vector<std::string>* allFunctionNames,
        std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas,
        std::vector<std::vector<std::string>>* allParameterNames
    );

    std::string GetName() { return name; }

    void SetAllFunctionPointers(
        std::vector<std::string>* allFunctionNames, 
        std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas,
        std::vector<std::vector<std::string>>* allParameterNames
    ) {
        this->allFunctionNames = allFunctionNames;
        this->allFormulas = allFormulas;
        this->allParameterNames = allParameterNames;
    }

    Number Calculate(
        std::vector<Number> arguments,
        Calc_main& calc_main
    );

};

void DefineUserFunction(
    std::string name,
    std::vector<std::string> parameterNames,
    std::vector<std::unique_ptr<CalculationPart>> formula,
    std::vector<std::string>* allFunctionNames,
    std::vector<std::vector<std::unique_ptr<CalculationPart>>>* allFormulas,
    std::vector<std::vector<std::string>>* allParameterNames
);


class DefineEqualSign : public CalculationPart {
public:
    DefineEqualSign() = default;
};



#endif // USERFUNCTIONS_HPP