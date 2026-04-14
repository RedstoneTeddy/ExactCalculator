#include "split_string.hpp"

#include <vector>
#include <string>
#include <memory>

#include "calculation/base_structures.hpp"
#include "calculation/number.hpp"
#include "calculation/equal_sign.hpp"
#include "calculation/variables.hpp"
#include "calculation/addition.hpp"
#include "calculation/subtraction.hpp"
#include "calculation/multiplication.hpp"
#include "calculation/division.hpp"



std::vector<std::unique_ptr<CalculationPart>> SplitString(std::string input, int maxSignificant) {
    std::vector<std::unique_ptr<CalculationPart>> parts;

    std::string currentPart = "";


    for (char c : input) {
        const bool isExponentSign =
            (c == '+' || c == '-') &&
            !currentPart.empty() &&
            (currentPart.back() == 'e' || currentPart.back() == 'E');

        if (isExponentSign) {
            currentPart += c;
            continue;
        }


        // +, -, *
        if (c == '+') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Addition>());
        }
        else if (c == '-') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Subtraction>());
        }
        else if (c == '*') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Multiplication>());
        }
        else if (c == '/') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Division>());
        }

        // Brackets
        else if (c == '(') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Bracket>(true));
        }
        else if (c == ')') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Bracket>(false));
        }

        else if (c == '=') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<EqualSign>());
        }

        // Else: Append to current part for number or a function name
        else {
            currentPart += c;
        }
    }

    HandleOperator(parts, currentPart, maxSignificant);

    return parts;
}

void HandleOperator(std::vector<std::unique_ptr<CalculationPart>>& parts, std::string& currentPart, int maxSignificant) {
    if (currentPart.empty()) {
        return;
    }

    // Functions must start with a capital letter, otherwise treated as a variable
    // Functions
    if (currentPart[0] >= 'A' && currentPart[0] <= 'Z') {
        // Handle functions
        // Constants
        if (currentPart == "Pi" || currentPart == "E" || currentPart == "G" || currentPart == "C" || currentPart == "U") {
            Constant constant(currentPart, maxSignificant);
            parts.push_back(std::make_unique<Constant>(constant));
        }

        // Treat unknown functions as variables, so they can be defined by the user
        else {
            Variable var(currentPart, nullptr, nullptr);
            parts.push_back(std::make_unique<Variable>(var));
        }


    // Variables
    } else if (currentPart[0] >= 'a' && currentPart[0] <= 'z') {
        // Handle variables
        Variable var(currentPart, nullptr, nullptr);
        parts.push_back(std::make_unique<Variable>(var));

    
    // Numbers
    } else {
        // Handle numbers
        Number num(maxSignificant);
        num.SetFromString(currentPart);
        parts.push_back(std::make_unique<Number>(num));
    }


    currentPart = "";
}