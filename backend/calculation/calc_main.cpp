#include "calc_main.hpp"
#include "number.hpp"
#include "base_structures.hpp"

#include <memory>
#include "addition.hpp"


Number Calc_main::Calculate_part(std::vector<std::unique_ptr<CalculationPart>>& calculation_parts) {


    // Handle +
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (Addition* addition = dynamic_cast<Addition*>(part.get())) {
            if (i == 0 || i == calculation_parts.size() - 1) {
                // Error, operator at start or end of expression
                return Number(0);
            }

            std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
            std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];

            if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
                if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {
                    Number result = addition->Calculate(*leftNum, *rightNum);

                    // Replace left part with result, remove operator and right part
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i); // Remove operator
                    calculation_parts.erase(calculation_parts.begin() + i); // Remove right part    
                    i--; // Move back index to account for removed parts
                } else {
                    // Error, right part is not a number
                    return Number(0);
                }
            } else {
                // Error, left part is not a number
                return Number(0);
            }                    
        }
    }


    std::unique_ptr<CalculationPart>& finalResult = calculation_parts[0];
    if (Number* finalNum = dynamic_cast<Number*>(finalResult.get())) {
        return *finalNum;
    } else {
        // Error, final result is not a number
        return Number(0);
    }
}