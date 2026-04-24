#include "sum.hpp"


#include "../CalculationError.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/calc_main.hpp"
#include "../calculation/clone_parts.hpp"

#include "../calculation/subtraction.hpp"
#include "../calculation/addition.hpp"
#include "../calculation/equal_sign.hpp"
#include "../calculation/variables.hpp"

#include <vector>
#include <memory>


Number Sum::Calculate(
    std::vector<std::unique_ptr<CalculationPart>>& from,
    std::vector<std::unique_ptr<CalculationPart>>& to,
    std::vector<std::unique_ptr<CalculationPart>>& formula,
    Calc_main& calc_main
) {
    std::unique_ptr<CalculationPart>& variablePart = from[0];
    if (Variable* variable = dynamic_cast<Variable*>(variablePart.get())) {
        // Valid variable assignment for summation index, continue with calculation
        std::string variableName = variable->GetName();
        std::vector<std::unique_ptr<CalculationPart>> formulaTemplate = CloneCalculationParts(formula);

        Number fromValue = calc_main.Calculate_part(from);
        Number toValue = calc_main.Calculate_part(to);

        Number result(fromValue.GetMaxSignificant());

        fromValue.CorrectForSignificance();
        toValue.CorrectForSignificance();

        // Check if from and to values have after-comma-digits
        if ((fromValue.GetExponent() - fromValue.GetDigits().size()) < 0) {
            throw CalculationError("The 'from' value in the Sum function cannot have after-comma digits.", ErrorType::SyntaxError);
        }
        if ((toValue.GetExponent() - toValue.GetDigits().size()) < 0) {
            throw CalculationError("The 'to' value in the Sum function cannot have after-comma digits.", ErrorType::SyntaxError);
        }

        // Calculate number of iterations
        Subtraction sub;
        Addition add;

        Number NumOne(fromValue.GetMaxSignificant());
        NumOne.SetNumber(false, {1}, 0);
        Number temp_value = sub.Calculate(toValue, fromValue);
        Number iterations = add.Calculate(temp_value, NumOne);

        // Check if iterations is a non-negative integer
        if (iterations.GetIsNegative() || (iterations.GetExponent() - iterations.GetDigits().size()) < 0) {
            throw CalculationError("The number of iterations in the Sum function must be a non-negative integer. Check the 'from' and 'to' values.", ErrorType::SyntaxError);
        }

        int iterationsInt = 0;
        for (int digit : iterations.GetDigits()) {
            iterationsInt = iterationsInt * 10 + digit;
        }

        // Calculate sum
        for (int i = 0; i < iterationsInt; i++) {
            std::vector<std::unique_ptr<CalculationPart>> currentFormula = CloneCalculationParts(formulaTemplate);
            Number currentCalculatedValue = calc_main.Calculate_part(currentFormula);
            result = add.Calculate(result, currentCalculatedValue);

            // Increment variable
            std::vector<std::unique_ptr<CalculationPart>> incrementParts;
            incrementParts.push_back(std::make_unique<Variable>(variableName, nullptr, nullptr));
            incrementParts.push_back(std::make_unique<EqualSign>());
            incrementParts.push_back(std::make_unique<Variable>(variableName, nullptr, nullptr));
            incrementParts.push_back(std::make_unique<Addition>());
            incrementParts.push_back(std::make_unique<Number>(NumOne));
            calc_main.Calculate_part(incrementParts);
        }

        return result;
    } else {
        throw CalculationError("The first argument of the Sum function must be a variable assignment, e.g., i=1.", ErrorType::SyntaxError);
    }
}

