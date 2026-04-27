#include "product.hpp"


#include "../CalculationError.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/calc_main.hpp"
#include "../clone_parts.hpp"

#include "../calculation/subtraction.hpp"
#include "../calculation/addition.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/equal_sign.hpp"
#include "../calculation/variables.hpp"

#include <vector>
#include <memory>
#include <limits>


Number Product::Calculate(
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
        result.SetNumber(false, {1}, 0); // Initialize result to 1 for multiplication

        fromValue.CorrectForSignificance();
        toValue.CorrectForSignificance();

        // Check if from and to values have after-comma-digits
        if ((fromValue.GetExponent() - fromValue.GetDigits().size()) < 0) {
            throw CalculationError("The 'from' value in the Product function cannot have after-comma digits.", ErrorType::SyntaxError);
        }
        if ((toValue.GetExponent() - toValue.GetDigits().size()) < 0) {
            throw CalculationError("The 'to' value in the Product function cannot have after-comma digits.", ErrorType::SyntaxError);
        }

        // Calculate number of iterations
        Subtraction sub;
        Addition add;
        Multiplication mul;

        Number NumOne(fromValue.GetMaxSignificant());
        NumOne.SetNumber(false, {1}, 0);
        Number temp_value = sub.Calculate(toValue, fromValue);
        Number iterations = add.Calculate(temp_value, NumOne);

        // Check if iterations is a non-negative integer
        if (iterations.GetIsNegative() || (iterations.GetExponent() - iterations.GetDigits().size()) < 0) {
            throw CalculationError("The number of iterations in the Product function must be a non-negative integer. Check the 'from' and 'to' values.", ErrorType::SyntaxError);
        }

        long long iterationsLL = 0;
        for (int digit : iterations.GetDigits()) {
            if (iterationsLL > (std::numeric_limits<long long>::max() - digit) / 10) {
                throw CalculationError("The number of iterations in the Product function is too large.", ErrorType::SyntaxError);
            }
            iterationsLL = iterationsLL * 10 + digit;
        }

        const int trailingZeros = iterations.GetExponent() + 1 - static_cast<int>(iterations.GetDigits().size());
        for (int i = 0; i < trailingZeros; i++) {
            if (iterationsLL > std::numeric_limits<long long>::max() / 10) {
                throw CalculationError("The number of iterations in the Product function is too large.", ErrorType::SyntaxError);
            }
            iterationsLL *= 10;
        }

        if (iterationsLL > std::numeric_limits<int>::max()) {
            throw CalculationError("The number of iterations in the Product function is too large.", ErrorType::SyntaxError);
        }
        int iterationsInt = static_cast<int>(iterationsLL);

        // Calculate product
        for (int i = 0; i < iterationsInt; i++) {
            std::vector<std::unique_ptr<CalculationPart>> currentFormula = CloneCalculationParts(formulaTemplate);
            Number currentCalculatedValue = calc_main.Calculate_part(currentFormula);
            result = mul.Calculate(result, currentCalculatedValue);

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
        throw CalculationError("The first argument of the Product function must be a variable assignment, e.g., i=1.", ErrorType::SyntaxError);
    }
}

