#include "calc_main.hpp"
#include "number.hpp"
#include "base_structures.hpp"

#include <memory>
#include <utility>
#include "addition.hpp"
#include "subtraction.hpp"
#include "multiplication.hpp"
#include "division.hpp"


Number Calc_main::Calculate_part(std::vector<std::unique_ptr<CalculationPart>>& calculation_parts) {


    // Handle ( and )
    int openBracket = -1;
    int nestedCounter = 0;
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (Bracket* leftBracket = dynamic_cast<Bracket*>(part.get())) {
            if (leftBracket->isOpen ) {
                if (nestedCounter == 0) {
                    openBracket = i; // Mark the position of the open bracket
                }   
                nestedCounter++;
            } else {
                nestedCounter--;
                if (openBracket != -1 && nestedCounter == 0) {
                    std::vector<std::unique_ptr<CalculationPart>> subParts;
                    for (int j = openBracket + 1; j < i; j++) {
                        subParts.push_back(std::move(calculation_parts[j]));
                    }
                    // Recursively calculate the result of the sub-expression
                    Number subResult = Calculate_part(subParts);
                    // Replace the bracketed expression with its result
                    calculation_parts[openBracket] = std::make_unique<Number>(subResult);
                    // Remove the processed parts
                    calculation_parts.erase(calculation_parts.begin() + openBracket + 1, calculation_parts.begin() + i + 1);
                    i = openBracket; // Adjust index to account for removed parts
                    openBracket = -1; // Reset open bracket index
                }
            }
        }
    }




    // Handle * and /
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i != 0 && i != calculation_parts.size() - 1) {
            std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
            std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
            if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
                if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {

                    if (Multiplication* multiplication = dynamic_cast<Multiplication*>(part.get())) {
                        Number result = multiplication->Calculate(*leftNum, *rightNum);

                        // Replace left part with result, remove operator and right part
                        leftPart = std::make_unique<Number>(result);
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove operator
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove right part    
                        i--; // Move back index to account for removed parts
                        
                    } 
                    else if (Division* division = dynamic_cast<Division*>(part.get())) {
                        Number result = division->Calculate(*leftNum, *rightNum);

                        // Replace left part with result, remove operator and right part
                        leftPart = std::make_unique<Number>(result);
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove operator
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove right part    
                        i--; // Move back index to account for removed parts
                    }

                }
            }
        }    
    }
    


    // Handle + and -
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i != 0 && i != calculation_parts.size() - 1) {
            std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
            std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
            if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
                if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {

                    if (Addition* addition = dynamic_cast<Addition*>(part.get())) {
                        Number result = addition->Calculate(*leftNum, *rightNum);

                        // Replace left part with result, remove operator and right part
                        leftPart = std::make_unique<Number>(result);
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove operator
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove right part    
                        i--; // Move back index to account for removed parts
                        
                    } else if (Subtraction* subtraction = dynamic_cast<Subtraction*>(part.get())) {
                        Number result = subtraction->Calculate(*leftNum, *rightNum);

                        // Replace left part with result, remove operator and right part
                        leftPart = std::make_unique<Number>(result);
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove operator
                        calculation_parts.erase(calculation_parts.begin() + i); // Remove right part    
                        i--; // Move back index to account for removed parts
                    }

                }
            }
        }    
    }






    // Get final result
    std::unique_ptr<CalculationPart>& finalResult = calculation_parts[0];
    if (Number* finalNum = dynamic_cast<Number*>(finalResult.get())) {
        return *finalNum;
    } else {
        // Error, final result is not a number
        return Number(0);
    }
}