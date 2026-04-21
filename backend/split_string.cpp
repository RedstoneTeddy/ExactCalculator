#include "split_string.hpp"

#include <vector>
#include <string>
#include <memory>
#include <cctype>

#include "CalculationError.hpp"

#include "calculation/base_structures.hpp"
#include "calculation/number.hpp"
#include "calculation/equal_sign.hpp"
#include "calculation/variables.hpp"

#include "functions/constants.hpp"
#include "functions/factorial.hpp"
#include "functions/root.hpp"
#include "functions/trigonometric.hpp"
#include "functions/logarithmic.hpp"

#include "calculation/addition.hpp"
#include "calculation/subtraction.hpp"
#include "calculation/multiplication.hpp"
#include "calculation/division.hpp"
#include "calculation/exponent.hpp"

namespace {
bool IsValidIdentifier(const std::string& token) {
    if (token.empty()) {
        return false;
    }

    for (char c : token) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_')) {
            return false;
        }
    }

    return true;
}
}



std::vector<std::unique_ptr<CalculationPart>> SplitString(std::string input, int maxSignificant, int rootSignificant) {
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
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Addition>());
        }
        else if (c == '-') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Subtraction>());
        }
        else if (c == '*') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Multiplication>());
        }
        else if (c == '/') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Division>());
        }
        else if (c == '^') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Exponent>(rootSignificant));
        }



        // Brackets
        else if (c == '(') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Bracket>(true, false));
        }
        else if (c == ')') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Bracket>(false, false));
        }
        else if (c == '{') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Bracket>(true, true));
        }
        else if (c == '}') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<Bracket>(false, true));
        }

        else if (c == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<EqualSign>());
        }
        else if (c == ',') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant);
            parts.push_back(std::make_unique<CommaSeparator>());
        }



        // Else: Append to current part for number or a function name
        else {
            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '.') {
                throw CalculationError("The expression contains an unsupported character.", ErrorType::SyntaxError);
            }
            currentPart += c;
        }
    }

    HandleOperator(parts, currentPart, maxSignificant, rootSignificant);

    return parts;
}

void HandleOperator(std::vector<std::unique_ptr<CalculationPart>>& parts, std::string& currentPart, int maxSignificant, int rootSignificant) {
    if (currentPart.empty()) {
        return;
    }

    // Functions must start with a capital letter, otherwise treated as a variable
    // Functions
    if (currentPart[0] >= 'A' && currentPart[0] <= 'Z') {
        if (!IsValidIdentifier(currentPart)) {
            throw CalculationError("Function names may only contain letters, digits, and underscores.", ErrorType::SyntaxError);
        }

        // Handle functions
        // Constants
        if (currentPart == "Pi" || currentPart == "E" || currentPart == "G" || currentPart == "C" || currentPart == "U") {
            Constant constant(currentPart, maxSignificant);
            parts.push_back(std::make_unique<Constant>(constant));
        }

        // Factorial
        else if (currentPart == "Fac" || currentPart == "Factorial") {
            Factorial factorial;
            parts.push_back(std::make_unique<Factorial>(factorial));
        }

        // Square root
        else if (currentPart == "Sqrt") {
            SquareRoot r;
            parts.push_back(std::make_unique<SquareRoot>(r));
        }

        // n-th Root
        else if (currentPart == "Root") {
            Root r;
            parts.push_back(std::make_unique<Root>(r));
        }

        // Sine
        else if (currentPart == "Sin" || currentPart == "Sine") {
            Sine sine;
            parts.push_back(std::make_unique<Sine>(sine));
        }

        // Cosine
        else if (currentPart == "Cos" || currentPart == "Cosine") {
            Cosine cosine;
            parts.push_back(std::make_unique<Cosine>(cosine));
        }

        // Tangent
        else if (currentPart == "Tan" || currentPart == "Tangent") {
            Tangent tangent;
            parts.push_back(std::make_unique<Tangent>(tangent));
        }

        // Arc sine
        else if (currentPart == "Asin" || currentPart == "ArcSin" || currentPart == "ArcSine") {
            ArcSine arcSine;
            parts.push_back(std::make_unique<ArcSine>(arcSine));
        }

        // Arc cosine
        else if (currentPart == "Acos" || currentPart == "ArcCos" || currentPart == "ArcCosine") {
            ArcCosine arcCosine;
            parts.push_back(std::make_unique<ArcCosine>(arcCosine));
        }

        // Arc tangent
        else if (currentPart == "Atan" || currentPart == "ArcTan" || currentPart == "ArcTangent") {
            ArcTangent arcTangent;
            parts.push_back(std::make_unique<ArcTangent>(arcTangent));
        }

        // Logarithm with base
        else if (currentPart == "Log" || currentPart == "Logarithm") {
            Logarithm logarithm;
            parts.push_back(std::make_unique<Logarithm>(logarithm));
        }

        // Natural logarithm
        else if (currentPart == "Ln" || currentPart == "NaturalLog" || currentPart == "NaturalLogarithm") {
            NaturalLogarithm naturalLogarithm;
            parts.push_back(std::make_unique<NaturalLogarithm>(naturalLogarithm));
        }

        // Treat unknown functions as variables, so they can be defined by the user
        else {
            Variable var(currentPart, nullptr, nullptr);
            parts.push_back(std::make_unique<Variable>(var));
        }


    // Variables
    } else if (currentPart[0] >= 'a' && currentPart[0] <= 'z') {
        if (!IsValidIdentifier(currentPart)) {
            throw CalculationError("Variable names may only contain letters, digits, and underscores.", ErrorType::SyntaxError);
        }

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