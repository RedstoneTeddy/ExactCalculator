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
#include "modulo.hpp"
#include "exponent.hpp"
#include "variables.hpp"
#include "equal_sign.hpp"

#include "../functions/constants.hpp"
#include "../functions/factorial.hpp"
#include "../functions/root.hpp"
#include "../functions/trigonometric.hpp"
#include "../functions/logarithmic.hpp"
#include "../functions/sum.hpp"
#include "../functions/product.hpp"
#include "../functions/minmax.hpp"
#include "../functions/combinatorics.hpp"
#include "../functions/round.hpp"
#include "../functions/boolean.hpp"
#include "../functions/prime.hpp"   
#include "../functions/random.hpp"

#include "../CalculationError.hpp"



Calc_main::Calc_main() {
    // varNumbers and varNames are initialized as empty vectors by default
}


std::vector<Number> Calc_main::extractNumbers(int i, int openFunctionBracket, std::vector<std::unique_ptr<CalculationPart>>& calculation_parts) {
    
    std::vector<std::vector<std::unique_ptr<CalculationPart>>> subParts = extractSubCalculations(i, openFunctionBracket, calculation_parts);
    
    std::vector<Number> functionArguments;
    for (std::vector<std::unique_ptr<CalculationPart>>& subPart : subParts) {
        Number argumentValue = Calculate_part(subPart);
        functionArguments.push_back(argumentValue);
    }
    return functionArguments;
}

std::vector<std::vector<std::unique_ptr<CalculationPart>>> Calc_main::extractSubCalculations(int i, int openFunctionBracket, std::vector<std::unique_ptr<CalculationPart>>& calculation_parts) {
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
    
    return subParts;
}




Number Calc_main::Calculate_part(std::vector<std::unique_ptr<CalculationPart>>& calculation_parts) {
    if (calculation_parts.empty()) {
        throw CalculationError("The expression is empty.", ErrorType::SyntaxError);
    }

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
                VARIABLES_HPP::SetVariable(variable->GetName(), subResult, &varNumbers, &varNames);
                return subResult;
            }
        }
    }




    // Handle functions
    int openFunctionBracket = -1;
    int nestedFunctionCounter = 0;

    auto requireArgumentCount = [](const std::vector<Number>& args, std::size_t expected, const std::string& functionName) {
        if (args.size() != expected) {
            throw CalculationError("Function '" + functionName + "' requires exactly " + std::to_string(expected) + " argument(s).", ErrorType::SyntaxError);
        }
    };

    auto replaceFunctionCallWithResult = [&](std::unique_ptr<CalculationPart>& functionPart, const Number& result, int& closeBracketIndex) {
        functionPart = std::make_unique<Number>(result);

        // Remove the full "{...}" argument region, regardless of expression complexity.
        const int removeCount = closeBracketIndex - openFunctionBracket + 1;
        calculation_parts.erase(
            calculation_parts.begin() + openFunctionBracket,
            calculation_parts.begin() + openFunctionBracket + removeCount
        );

        // Continue scanning from the function result token.
        closeBracketIndex = openFunctionBracket - 1;
    };

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
                    if (nestedFunctionCounter < 0) {
                        throw CalculationError("Found a closing function bracket without a matching opening bracket.", ErrorType::SyntaxError);
                    }

                    if (openFunctionBracket != -1 && nestedFunctionCounter == 0) {                        

                        // Check which function it is and calculate accordingly
                        if (openFunctionBracket > 0) {
                            std::unique_ptr<CalculationPart>& functionPart = calculation_parts[openFunctionBracket - 1];
                            
                            // Factorial
                            if (Factorial* factorial = dynamic_cast<Factorial*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Factorial");
                                Number result = factorial->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Square root
                            else if (SquareRoot* squareRoot = dynamic_cast<SquareRoot*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Sqrt");
                                Number result = squareRoot->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Sine
                            else if (Sine* sine = dynamic_cast<Sine*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Sin");
                                Number result = sine->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Cosine
                            else if (Cosine* cosine = dynamic_cast<Cosine*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Cos");
                                Number result = cosine->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Tangent
                            else if (Tangent* tangent = dynamic_cast<Tangent*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Tan");
                                Number result = tangent->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Arc sine
                            else if (ArcSine* arcSine = dynamic_cast<ArcSine*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Asin");
                                Number result = arcSine->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Arc cosine
                            else if (ArcCosine* arcCosine = dynamic_cast<ArcCosine*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Acos");
                                Number result = arcCosine->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Arc tangent
                            else if (ArcTangent* arcTangent = dynamic_cast<ArcTangent*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Atan");
                                Number result = arcTangent->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Natural logarithm
                            else if (NaturalLogarithm* naturalLogarithm = dynamic_cast<NaturalLogarithm*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Ln");
                                Number result = naturalLogarithm->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Logarithm with base
                            else if (Logarithm* logarithm = dynamic_cast<Logarithm*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "Log");
                                Number result = logarithm->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // n-th Root
                            else if (Root* root = dynamic_cast<Root*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "Root");
                                Number result = root->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            // Sum
                            else if (Sum* sum = dynamic_cast<Sum*>(functionPart.get())) {
                                std::vector<std::vector<std::unique_ptr<CalculationPart>>> subCalculations = extractSubCalculations(i, openFunctionBracket, calculation_parts);
                                if (subCalculations.size() != 3) {
                                    throw CalculationError("Function 'Sum' requires exactly 3 arguments, example: Sum{i=1,10,i^2}", ErrorType::SyntaxError);
                                }
                                Number result = sum->Calculate(subCalculations[0], subCalculations[1], subCalculations[2], *this);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }
                            
                            // Product
                            else if (Product* product = dynamic_cast<Product*>(functionPart.get())) {
                                std::vector<std::vector<std::unique_ptr<CalculationPart>>> subCalculations = extractSubCalculations(i, openFunctionBracket, calculation_parts);
                                if (subCalculations.size() != 3) {
                                    throw CalculationError("Function 'Product' requires exactly 3 arguments, example: Product{i=1,10,i}", ErrorType::SyntaxError);
                                }
                                Number result = product->Calculate(subCalculations[0], subCalculations[1], subCalculations[2], *this);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Min* min = dynamic_cast<Min*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "Min");
                                Number result = min->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Max* max = dynamic_cast<Max*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "Max");
                                Number result = max->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }
                            
                            else if (nPr* npr = dynamic_cast<nPr*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "nPr");
                                Number result = npr->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (nCr* ncr = dynamic_cast<nCr*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "nCr");
                                Number result = ncr->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Abs* abs = dynamic_cast<Abs*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Abs");
                                Number result = abs->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Round* round = dynamic_cast<Round*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Round");
                                Number result = round->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Ceil* ceil = dynamic_cast<Ceil*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Ceil");
                                Number result = ceil->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Floor* floor = dynamic_cast<Floor*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Floor");
                                Number result = floor->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (BooleanIf* ifFunction = dynamic_cast<BooleanIf*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 3, "If");
                                Number result = ifFunction->Calculate(functionArguments[0], functionArguments[1], functionArguments[2]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Gcd* gcd = dynamic_cast<Gcd*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "Gcd");
                                Number result = gcd->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Lcm* lcm = dynamic_cast<Lcm*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 2, "Lcm");
                                Number result = lcm->Calculate(functionArguments[0], functionArguments[1]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (Nthprime* nthprime = dynamic_cast<Nthprime*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "Nthprime");
                                Number result = nthprime->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else if (RandomInt* randomInt = dynamic_cast<RandomInt*>(functionPart.get())) {
                                std::vector<Number> functionArguments = extractNumbers(i, openFunctionBracket, calculation_parts);
                                requireArgumentCount(functionArguments, 1, "RandomInt");
                                Number result = randomInt->Calculate(functionArguments[0]);
                                replaceFunctionCallWithResult(functionPart, result, i);
                            }

                            else {
                                throw CalculationError("Function brackets must follow a valid function name.", ErrorType::SyntaxError);
                            }
                        } else {
                            throw CalculationError("Function brackets must follow a valid function name.", ErrorType::SyntaxError);
                        }

                        openFunctionBracket = -1; // Reset open function bracket index
                    }
                }
            }
        }
    }

    if (nestedFunctionCounter != 0) {
        throw CalculationError("A function call is missing a closing bracket.", ErrorType::SyntaxError);
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
                if (nestedCounter < 0) {
                    throw CalculationError("Found a closing bracket without a matching opening bracket.", ErrorType::SyntaxError);
                }

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

    if (nestedCounter != 0) {
        throw CalculationError("A bracketed expression is missing a closing bracket.", ErrorType::SyntaxError);
    }

    //  check if all Bracket-threads finished and returned their result, which will then get stored






    // Handle !
    for (int i = static_cast<int>(calculation_parts.size()) - 2; i >= 0; i--) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (BooleanNot* booleanNot = dynamic_cast<BooleanNot*>(part.get())) {
            std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
            Number* rightNum = dynamic_cast<Number*>(rightPart.get());
            if (!rightNum) {
                throw CalculationError("The right operand of the logical NOT operator (!) must be a number.", ErrorType::SyntaxError);
            }

            Number result = booleanNot->Calculate(*rightNum);
            part = std::make_unique<Number>(result);
            calculation_parts.erase(calculation_parts.begin() + i + 1);
        }
    }

    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<BooleanNot*>(calculation_parts[i].get())) {
            throw CalculationError("The logical NOT operator (!) could not be evaluated. Check that the operand is a valid number.", ErrorType::SyntaxError);
        }
    }





    // Handle ^
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];
        if (Exponent* exponent = dynamic_cast<Exponent*>(part.get())) {
            if (i == 0 || i == calculation_parts.size() - 1) {
                throw CalculationError("The exponentiation operator (^) cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
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
                } else {
                    throw CalculationError("The right operand of the exponentiation operator (^) must be a number.", ErrorType::SyntaxError);
                }
            } else {
                throw CalculationError("The left operand of the exponentiation operator (^) must be a number.", ErrorType::SyntaxError);
            }
        }
    }

    // Check for any remaining exponent operators that couldn't be reduced
    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<Exponent*>(calculation_parts[i].get())) {
            throw CalculationError("The exponentiation operator (^) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }


    

    // Handle * and / and %
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i == 0 || i == calculation_parts.size() - 1) {
            if (dynamic_cast<Multiplication*>(part.get()) || dynamic_cast<Division*>(part.get())) {
                throw CalculationError("Multiplication and division operators cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
            continue;
        }

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
                else if (Modulo* modulo = dynamic_cast<Modulo*>(part.get())) {
                    Number result = modulo->Calculate(*leftNum, *rightNum);

                    // Replace left part with result, remove operator and right part
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i); // Remove operator
                    calculation_parts.erase(calculation_parts.begin() + i); // Remove right part    
                    i--; // Move back index to account for removed parts
                }
            } else {
                if (dynamic_cast<Multiplication*>(part.get())) {
                    throw CalculationError("The right operand of the multiplication operator (*) must be a number.", ErrorType::SyntaxError);
                } else if (dynamic_cast<Division*>(part.get())) {
                    throw CalculationError("The right operand of the division operator (/) must be a number.", ErrorType::SyntaxError);
                }
            }
        } else {
            if (dynamic_cast<Multiplication*>(part.get())) {
                throw CalculationError("The left operand of the multiplication operator (*) must be a number.", ErrorType::SyntaxError);
            } else if (dynamic_cast<Division*>(part.get())) {
                throw CalculationError("The left operand of the division operator (/) must be a number.", ErrorType::SyntaxError);
            }
        }    
    }

    // Check for any remaining multiplication/division operators that couldn't be reduced
    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<Multiplication*>(calculation_parts[i].get())) {
            throw CalculationError("The multiplication operator (*) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
        if (dynamic_cast<Division*>(calculation_parts[i].get())) {
            throw CalculationError("The division operator (/) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }
    


    // Handle + and -
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i == 0 || i == calculation_parts.size() - 1) {
            if (dynamic_cast<Addition*>(part.get()) || dynamic_cast<Subtraction*>(part.get())) {
                throw CalculationError("Addition and subtraction operators cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
            continue;
        }

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
            } else {
                if (dynamic_cast<Addition*>(part.get())) {
                    throw CalculationError("The right operand of the addition operator (+) must be a number.", ErrorType::SyntaxError);
                } else if (dynamic_cast<Subtraction*>(part.get())) {
                    throw CalculationError("The right operand of the subtraction operator (-) must be a number.", ErrorType::SyntaxError);
                }
            }
        } else {
            if (dynamic_cast<Addition*>(part.get())) {
                throw CalculationError("The left operand of the addition operator (+) must be a number.", ErrorType::SyntaxError);
            } else if (dynamic_cast<Subtraction*>(part.get())) {
                throw CalculationError("The left operand of the subtraction operator (-) must be a number.", ErrorType::SyntaxError);
            }
        }    
    }

    // Check for any remaining addition/subtraction operators that couldn't be reduced
    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<Addition*>(calculation_parts[i].get())) {
            throw CalculationError("The addition operator (+) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
        if (dynamic_cast<Subtraction*>(calculation_parts[i].get())) {
            throw CalculationError("The subtraction operator (-) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }






    // Handle <, <=, > and >=
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i == 0 || i == calculation_parts.size() - 1) {
            if (dynamic_cast<BooleanLess*>(part.get()) || dynamic_cast<BooleanLessEquals*>(part.get()) || dynamic_cast<BooleanGreater*>(part.get()) || dynamic_cast<BooleanGreaterEquals*>(part.get())) {
                throw CalculationError("Comparison operators cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
            continue;
        }

        std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
        std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
        if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
            if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {
                if (BooleanLess* less = dynamic_cast<BooleanLess*>(part.get())) {
                    Number result = less->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                } else if (BooleanLessEquals* lessEquals = dynamic_cast<BooleanLessEquals*>(part.get())) {
                    Number result = lessEquals->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                } else if (BooleanGreater* greater = dynamic_cast<BooleanGreater*>(part.get())) {
                    Number result = greater->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                } else if (BooleanGreaterEquals* greaterEquals = dynamic_cast<BooleanGreaterEquals*>(part.get())) {
                    Number result = greaterEquals->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                }
            } else {
                if (dynamic_cast<BooleanLess*>(part.get()) || dynamic_cast<BooleanLessEquals*>(part.get()) || dynamic_cast<BooleanGreater*>(part.get()) || dynamic_cast<BooleanGreaterEquals*>(part.get())) {
                    throw CalculationError("A comparison operator requires numeric operands.", ErrorType::SyntaxError);
                }
            }
        } else {
            if (dynamic_cast<BooleanLess*>(part.get()) || dynamic_cast<BooleanLessEquals*>(part.get()) || dynamic_cast<BooleanGreater*>(part.get()) || dynamic_cast<BooleanGreaterEquals*>(part.get())) {
                throw CalculationError("A comparison operator requires numeric operands.", ErrorType::SyntaxError);
            }
        }
    }

    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<BooleanLess*>(calculation_parts[i].get()) || dynamic_cast<BooleanLessEquals*>(calculation_parts[i].get()) || dynamic_cast<BooleanGreater*>(calculation_parts[i].get()) || dynamic_cast<BooleanGreaterEquals*>(calculation_parts[i].get())) {
            throw CalculationError("A comparison operator could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }






    // Handle == and !=
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i == 0 || i == calculation_parts.size() - 1) {
            if (dynamic_cast<BooleanEquals*>(part.get()) || dynamic_cast<BooleanNotEquals*>(part.get())) {
                throw CalculationError("Equality operators cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
            continue;
        }

        std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
        std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
        if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
            if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {
                if (BooleanEquals* equals = dynamic_cast<BooleanEquals*>(part.get())) {
                    Number result = equals->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                } else if (BooleanNotEquals* notEquals = dynamic_cast<BooleanNotEquals*>(part.get())) {
                    Number result = notEquals->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                }
            } else {
                if (dynamic_cast<BooleanEquals*>(part.get()) || dynamic_cast<BooleanNotEquals*>(part.get())) {
                    throw CalculationError("Equality operators require numeric operands.", ErrorType::SyntaxError);
                }
            }
        } else {
            if (dynamic_cast<BooleanEquals*>(part.get()) || dynamic_cast<BooleanNotEquals*>(part.get())) {
                throw CalculationError("Equality operators require numeric operands.", ErrorType::SyntaxError);
            }
        }
    }

    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<BooleanEquals*>(calculation_parts[i].get()) || dynamic_cast<BooleanNotEquals*>(calculation_parts[i].get())) {
            throw CalculationError("An equality operator could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }






    // Handle &&
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i == 0 || i == calculation_parts.size() - 1) {
            if (dynamic_cast<BooleanAnd*>(part.get())) {
                throw CalculationError("The logical AND operator (&&) cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
            continue;
        }

        std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
        std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
        if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
            if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {
                if (BooleanAnd* booleanAnd = dynamic_cast<BooleanAnd*>(part.get())) {
                    Number result = booleanAnd->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                }
            } else {
                if (dynamic_cast<BooleanAnd*>(part.get())) {
                    throw CalculationError("The logical AND operator (&&) requires numeric operands.", ErrorType::SyntaxError);
                }
            }
        } else {
            if (dynamic_cast<BooleanAnd*>(part.get())) {
                throw CalculationError("The logical AND operator (&&) requires numeric operands.", ErrorType::SyntaxError);
            }
        }
    }

    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<BooleanAnd*>(calculation_parts[i].get())) {
            throw CalculationError("The logical AND operator (&&) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }






    // Handle ||
    for (int i = 0; i < calculation_parts.size(); i++) {
        std::unique_ptr<CalculationPart>& part = calculation_parts[i];

        if (i == 0 || i == calculation_parts.size() - 1) {
            if (dynamic_cast<BooleanOr*>(part.get())) {
                throw CalculationError("The logical OR operator (||) cannot appear at the beginning or end of an expression.", ErrorType::SyntaxError);
            }
            continue;
        }

        std::unique_ptr<CalculationPart>& leftPart = calculation_parts[i - 1];
        std::unique_ptr<CalculationPart>& rightPart = calculation_parts[i + 1];
        if (Number* leftNum = dynamic_cast<Number*>(leftPart.get())) {
            if (Number* rightNum = dynamic_cast<Number*>(rightPart.get())) {
                if (BooleanOr* booleanOr = dynamic_cast<BooleanOr*>(part.get())) {
                    Number result = booleanOr->Calculate(*leftNum, *rightNum);
                    leftPart = std::make_unique<Number>(result);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    calculation_parts.erase(calculation_parts.begin() + i);
                    i--;
                }
            } else {
                if (dynamic_cast<BooleanOr*>(part.get())) {
                    throw CalculationError("The logical OR operator (||) requires numeric operands.", ErrorType::SyntaxError);
                }
            }
        } else {
            if (dynamic_cast<BooleanOr*>(part.get())) {
                throw CalculationError("The logical OR operator (||) requires numeric operands.", ErrorType::SyntaxError);
            }
        }
    }

    for (int i = 0; i < calculation_parts.size(); i++) {
        if (dynamic_cast<BooleanOr*>(calculation_parts[i].get())) {
            throw CalculationError("The logical OR operator (||) could not be evaluated. Check that both operands are valid numbers.", ErrorType::SyntaxError);
        }
    }






    // Get final result
    if (calculation_parts.size() != 1) {
        throw CalculationError("The expression could not be reduced to a single value.", ErrorType::SyntaxError);
    }

    std::unique_ptr<CalculationPart>& finalResult = calculation_parts[0];
    if (Number* finalNum = dynamic_cast<Number*>(finalResult.get())) {
        return *finalNum;
    } else {
        throw CalculationError("The expression could not be reduced to a numeric result.", ErrorType::SyntaxError);
    }
}