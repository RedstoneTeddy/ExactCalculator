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
#include "../functions/factorial.hpp"
#include "../functions/root.hpp"


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



    // Handle functions: factorial
    int openFunctionBracket = -1;
    int nestedFunctionCounter = 0;
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (Bracket* bracket = dynamic_cast<Bracket*>(part.get())) {
            if (bracket->isFunctionBracket) {
                if (bracket->isOpen) {
                    if (nestedFunctionCounter == 0) {
                        openFunctionBracket = i; // Mark the position of the open function bracket
                    }   
                    nestedFunctionCounter++;
                } else {
                    nestedFunctionCounter--;
                    if (openFunctionBracket != -1 && nestedFunctionCounter == 0) {
                        // Calculate the result of the inputs for the function
                        std::vector<std::vector<std::unique_ptr<CalculationPart>>> subParts;
                        int currentSubCalculationIndex = 0;
                        subParts.push_back(std::vector<std::unique_ptr<CalculationPart>>());
                        for (int j = openFunctionBracket + 1; j < i; j++) {
                            std::unique_ptr<CalculationPart>& currentPart = calculation_parts[j];
                            if (CommaSeparator* commaSeparator = dynamic_cast<CommaSeparator*>(currentPart.get())) {
                                // Start a new sub-calculation for the next argument
                                currentSubCalculationIndex++;
                                subParts.push_back(std::vector<std::unique_ptr<CalculationPart>>());
                            } else {
                                // Add part to the current sub-calculation
                                subParts[currentSubCalculationIndex].push_back(std::move(currentPart));
                            }
                        }
                        
                        std::vector<Number> functionArguments;
                        for (std::vector<std::unique_ptr<CalculationPart>>& subPart : subParts) {
                            Number argumentValue = Calculate_part(subPart);
                            functionArguments.push_back(argumentValue);
                        }

                        // Check which function it is and calculate accordingly
                        if (openFunctionBracket > 0) {
                            std::unique_ptr<CalculationPart>& functionPart = calculation_parts[openFunctionBracket - 1];
                            
                            // Factorial
                            if (Factorial* factorial = dynamic_cast<Factorial*>(functionPart.get())) {
                                Number result = factorial->Calculate(functionArguments[0]);

                                // Replace function part with result, remove brackets and inner parts
                                functionPart = std::make_unique<Number>(result);
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove open bracket
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove open bracket
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove close bracket    
                                i -= 3; // Move back index to account for removed parts
                            }

                            // Square root
                            else if (SquareRoot* squareRoot = dynamic_cast<SquareRoot*>(functionPart.get())) {
                                Number result = squareRoot->Calculate(functionArguments[0]);

                                // Replace function part with result, remove brackets and inner parts
                                functionPart = std::make_unique<Number>(result);
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove open bracket
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove open bracket
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove close bracket    
                                i -= 3; // Move back index to account for removed parts
                            }

                            // n-th Root
                            else if (Root* root = dynamic_cast<Root*>(functionPart.get())) {
                                Number result = root->Calculate(functionArguments[0], functionArguments[1]);

                                // Replace function part with result, remove brackets and inner parts
                                functionPart = std::make_unique<Number>(result);
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove open bracket
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove close bracket    
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove close bracket    
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove close bracket    
                                calculation_parts.erase(calculation_parts.begin() + openFunctionBracket); // Remove close bracket    
                                i -= 5; // Move back index to account for removed parts
                            }
                        }

                        openFunctionBracket = -1; // Reset open function bracket index
                    }
                }
            }
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