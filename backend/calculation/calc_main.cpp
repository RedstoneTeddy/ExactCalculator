#include "calc_main.hpp"
#include "number.hpp"
#include "base_structures.hpp"

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


Calc_main::Calc_main() {
    // varNumbers and varNames are initialized as empty vectors by default
}


Number Calc_main::Calculate_part(std::vector<std::unique_ptr<CalculationPart>>& calculation_parts) {

    // Check for variable = num, and return immediately
    if (calculation_parts.size() > 2) {
        std::unique_ptr<CalculationPart>& firstPart = calculation_parts[0];
        std::unique_ptr<CalculationPart>& secondPart = calculation_parts[1];
        if (Variable* variable = dynamic_cast<Variable*>(firstPart.get())) {
            if (EqualSign* equalSign = dynamic_cast<EqualSign*>(secondPart.get())) {

                // Calculate value to store in the variable
                std::vector<std::unique_ptr<CalculationPart>> subParts;
                for (int j = 2; j < calculation_parts.size(); j++) {
                    subParts.push_back(std::move(calculation_parts[j]));
                }
                // Recursively calculate the result of the sub-expression
                Number subResult = Calculate_part(subParts);


                // Store the result in the variable
                SetVariable(variable->GetName(), subResult, &varNumbers, &varNames);
                return subResult;
            }
        }
    }



    // Replace all variables & constants with their values
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (Variable* variable = dynamic_cast<Variable*>(part.get())) {
            variable->SetVectorPointers(&varNumbers, &varNames);
            Number value = variable->GetValue();
            value.CorrectForSignificance();
            part = std::make_unique<Number>(value);
        }
        else if (Constant* constant = dynamic_cast<Constant*>(part.get())) {
            Number value = constant->GetValue();
            value.CorrectForSignificance();
            part = std::make_unique<Number>(value);
        }
    }


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
                    // Start a new thread and later check if all threads finished and returned their result, which will then get stored
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

    //  check if all Bracket-threads finished and returned their result, which will then get stored





    // Handle ^
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (Exponent* exponent = dynamic_cast<Exponent*>(part.get())) {
            if (i != 0 && i != calculation_parts.size() - 1) {
                std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
                std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
                if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
                    if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {
                        Number result = exponent->Calculate(*leftNum, *rightNum);

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