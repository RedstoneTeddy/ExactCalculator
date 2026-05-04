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
#include "functions/sum.hpp"
#include "functions/product.hpp"
#include "functions/trigonometric.hpp"
#include "functions/logarithmic.hpp"
#include "functions/minmax.hpp"
#include "functions/combinatorics.hpp"
#include "functions/round.hpp"
#include "functions/boolean.hpp"
#include "functions/prime.hpp"
#include "functions/random.hpp"
#include "functions/userfunctions.hpp"

#include "calculation/addition.hpp"
#include "calculation/subtraction.hpp"
#include "calculation/multiplication.hpp"
#include "calculation/division.hpp"
#include "calculation/modulo.hpp"
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

bool IsScientificExponentSign(const std::string& token) {
    if (token.empty()) {
        return false;
    }

    char last = token.back();
    if (last != 'e' && last != 'E') {
        return false;
    }

    bool digitSeen = false;
    bool decimalSeen = false;
    for (std::size_t i = 0; i + 1 < token.size(); i++) {
        char c = token[i];
        if (c >= '0' && c <= '9') {
            digitSeen = true;
            continue;
        }
        if (c == '.') {
            if (decimalSeen) {
                return false;
            }
            decimalSeen = true;
            continue;
        }
        return false;
    }

    return digitSeen;
}
}



std::vector<std::unique_ptr<CalculationPart>> SplitString(std::string input, int maxSignificant, int rootSignificant) {
    std::vector<std::unique_ptr<CalculationPart>> parts;

    std::string currentPart = "";


    // Quick check if a function definition follows (:=)
    std::size_t definitionPos = std::string::npos;
    for (std::size_t index = 0; index < input.size(); index++) {
        char c = input[index];
        char next = index + 1 < input.size() ? input[index + 1] : '\0';
        if (c == ':' && next == '=') {
            definitionPos = index;
            break;
        }
    }




    for (std::size_t index = 0; index < input.size(); index++) {
        char c = input[index];
        char next = index + 1 < input.size() ? input[index + 1] : '\0';

        const bool isExponentSign =
            (c == '+' || c == '-') &&
            IsScientificExponentSign(currentPart);

        if (isExponentSign) {
            currentPart += c;
            continue;
        }

        const bool inFunctionDefinition = definitionPos != std::string::npos && index <= definitionPos;

        if (c == '=' && next == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanEquals>());
            index++;
            continue;
        }
        else if (c == '!' && next == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanNotEquals>());
            index++;
            continue;
        }
        else if (c == '<' && next == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanLessEquals>());
            index++;
            continue;
        }
        else if (c == '>' && next == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanGreaterEquals>());
            index++;
            continue;
        }
        else if (c == '&' && next == '&') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanAnd>());
            index++;
            continue;
        }
        else if (c == '|' && next == '|') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanOr>());
            index++;
            continue;
        }
        else if (definitionPos != std::string::npos && c == ':' && next == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<DefineEqualSign>());
            index++;
            continue;
        }


        // +, -, *, /, ^, %
        if (c == '+') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Addition>());
        }
        else if (c == '-') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Subtraction>());
        }
        else if (c == '*') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Multiplication>());
        }
        else if (c == '/') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Division>());
        }
        else if (c == '%') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Modulo>());
        }
        else if (c == '^') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Exponent>(rootSignificant));
        }
        else if (c == '!') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanNot>());
            continue;
        }
        else if (c == '<') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanLess>());
            continue;
        }
        else if (c == '>') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<BooleanGreater>());
            continue;
        }



        // Brackets
        else if (c == '(') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Bracket>(true, false));
        }
        else if (c == ')') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Bracket>(false, false));
        }
        else if (c == '{') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Bracket>(true, true));
        }
        else if (c == '}') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<Bracket>(false, true));
        }

        else if (c == '=') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
            parts.push_back(std::make_unique<EqualSign>());
        }
        else if (c == ',') {
            HandleOperator(parts, currentPart, maxSignificant, rootSignificant, inFunctionDefinition);
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

    HandleOperator(parts, currentPart, maxSignificant, rootSignificant, false);

    return parts;
}

void HandleOperator(
    std::vector<std::unique_ptr<CalculationPart>>& parts, 
    std::string& currentPart, 
    int maxSignificant, 
    int rootSignificant, 
    bool functionDefinition
) {
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

        // Sum
        else if (currentPart == "Sum") {
            Sum sum;
            parts.push_back(std::make_unique<Sum>(sum));
        }

        // Product
        else if (currentPart == "Product" || currentPart == "Prod") {
            Product product;
            parts.push_back(std::make_unique<Product>(product));
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

        // Min
        else if (currentPart == "Min") {
            Min min;
            parts.push_back(std::make_unique<Min>(min));
        }

        // Max
        else if (currentPart == "Max") {
            Max max;
            parts.push_back(std::make_unique<Max>(max));
        }

        // nPr
        else if (currentPart == "Npr" || currentPart == "NPr" || currentPart == "Perm") {
            nPr npr;
            parts.push_back(std::make_unique<nPr>(npr));
        }

        // nCr
        else if (currentPart == "Ncr" || currentPart == "NCr" || currentPart == "Choose") {
            nCr ncr;
            parts.push_back(std::make_unique<nCr>(ncr));
        }

        // Abs
        else if (currentPart == "Abs" || currentPart == "AbsoluteValue") {
            Abs abs;
            parts.push_back(std::make_unique<Abs>(abs));
        }

        // Round
        else if (currentPart == "Round") {
            Round round;
            parts.push_back(std::make_unique<Round>(round));
        }

        // Ceil
        else if (currentPart == "Ceil") {
            Ceil ceil;
            parts.push_back(std::make_unique<Ceil>(ceil));
        }

        // Floor
        else if (currentPart == "Floor") {
            Floor floor;
            parts.push_back(std::make_unique<Floor>(floor));
        }

        // If-Function
        else if (currentPart == "If" || currentPart == "Ifelse" || currentPart == "IfElse" || currentPart == "Elif") {
            BooleanIf ifFunction;
            parts.push_back(std::make_unique<BooleanIf>(ifFunction));
        }

        // GCD
        else if (currentPart == "Gcd" || currentPart == "GCD") {
            Gcd gcd;
            parts.push_back(std::make_unique<Gcd>(gcd));
        }

        // LCM
        else if (currentPart == "Lcm" || currentPart == "LCM") {
            Lcm lcm;
            parts.push_back(std::make_unique<Lcm>(lcm));
        }

        // Nth prime
        else if (currentPart == "Nthprime" || currentPart == "NthPrime") {
            Nthprime nthprime;
            parts.push_back(std::make_unique<Nthprime>(nthprime));
        }

        // RandomInt
        else if (currentPart == "RandomInt" || currentPart == "Randomint" || currentPart == "RandomInteger") {
            RandomInt randomInt;
            parts.push_back(std::make_unique<RandomInt>(randomInt));
        }

        // Treat unknown functions as variables, so they can be defined by the user
        else {
            if (functionDefinition) {
                UserFunction func(currentPart, nullptr, nullptr, nullptr);
                parts.push_back(std::make_unique<UserFunction>(func));
            } else {
                Variable var(currentPart, nullptr, nullptr);
                parts.push_back(std::make_unique<Variable>(var));
            }
        }


    // Variables
    } else if (currentPart[0] >= 'a' && currentPart[0] <= 'z') {
        if (!IsValidIdentifier(currentPart)) {
            throw CalculationError("Variable names may only contain letters, digits, and underscores.", ErrorType::SyntaxError);
        }

        // Handle variables and user-defined functions
        if (functionDefinition) {
            UserFunction func(currentPart, nullptr, nullptr, nullptr);
            parts.push_back(std::make_unique<UserFunction>(func));
        } else {
            Variable var(currentPart, nullptr, nullptr);
            parts.push_back(std::make_unique<Variable>(var));
        }

    
    // Numbers
    } else {
        // Handle numbers
        Number num(maxSignificant);
        num.SetFromString(currentPart);
        parts.push_back(std::make_unique<Number>(num));
    }


    currentPart = "";
}