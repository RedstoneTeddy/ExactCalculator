#include "clone_parts.hpp"

#include "CalculationError.hpp"

#include "calculation/number.hpp"
#include "calculation/variables.hpp"
#include "calculation/addition.hpp"
#include "calculation/subtraction.hpp"
#include "calculation/multiplication.hpp"
#include "calculation/division.hpp"
#include "calculation/exponent.hpp"
#include "calculation/equal_sign.hpp"

#include "functions/constants.hpp"
#include "functions/factorial.hpp"
#include "functions/root.hpp"
#include "functions/trigonometric.hpp"
#include "functions/logarithmic.hpp"
#include "functions/sum.hpp"
#include "functions/product.hpp"
#include "functions/minmax.hpp"

std::unique_ptr<CalculationPart> CloneCalculationPart(const CalculationPart& part) {
    if (const Number* number = dynamic_cast<const Number*>(&part)) {
        return std::make_unique<Number>(*number);
    }
    if (const Variable* variable = dynamic_cast<const Variable*>(&part)) {
        return std::make_unique<Variable>(*variable);
    }
    if (const Exponent* exponent = dynamic_cast<const Exponent*>(&part)) {
        return std::make_unique<Exponent>(*exponent);
    }
    if (const Division* division = dynamic_cast<const Division*>(&part)) {
        return std::make_unique<Division>(*division);
    }
    if (const Multiplication* multiplication = dynamic_cast<const Multiplication*>(&part)) {
        return std::make_unique<Multiplication>(*multiplication);
    }
    if (const Subtraction* subtraction = dynamic_cast<const Subtraction*>(&part)) {
        return std::make_unique<Subtraction>(*subtraction);
    }
    if (const Addition* addition = dynamic_cast<const Addition*>(&part)) {
        return std::make_unique<Addition>(*addition);
    }
    if (const EqualSign* equalSign = dynamic_cast<const EqualSign*>(&part)) {
        return std::make_unique<EqualSign>(*equalSign);
    }
    if (const Bracket* bracket = dynamic_cast<const Bracket*>(&part)) {
        return std::make_unique<Bracket>(*bracket);
    }
    if (const CommaSeparator* comma = dynamic_cast<const CommaSeparator*>(&part)) {
        return std::make_unique<CommaSeparator>(*comma);
    }
    if (const Constant* constant = dynamic_cast<const Constant*>(&part)) {
        return std::make_unique<Constant>(*constant);
    }
    if (const Factorial* factorial = dynamic_cast<const Factorial*>(&part)) {
        return std::make_unique<Factorial>(*factorial);
    }
    if (const SquareRoot* squareRoot = dynamic_cast<const SquareRoot*>(&part)) {
        return std::make_unique<SquareRoot>(*squareRoot);
    }
    if (const Root* root = dynamic_cast<const Root*>(&part)) {
        return std::make_unique<Root>(*root);
    }
    if (const Sine* sine = dynamic_cast<const Sine*>(&part)) {
        return std::make_unique<Sine>(*sine);
    }
    if (const Cosine* cosine = dynamic_cast<const Cosine*>(&part)) {
        return std::make_unique<Cosine>(*cosine);
    }
    if (const Tangent* tangent = dynamic_cast<const Tangent*>(&part)) {
        return std::make_unique<Tangent>(*tangent);
    }
    if (const ArcSine* arcSine = dynamic_cast<const ArcSine*>(&part)) {
        return std::make_unique<ArcSine>(*arcSine);
    }
    if (const ArcCosine* arcCosine = dynamic_cast<const ArcCosine*>(&part)) {
        return std::make_unique<ArcCosine>(*arcCosine);
    }
    if (const ArcTangent* arcTangent = dynamic_cast<const ArcTangent*>(&part)) {
        return std::make_unique<ArcTangent>(*arcTangent);
    }
    if (const Logarithm* logarithm = dynamic_cast<const Logarithm*>(&part)) {
        return std::make_unique<Logarithm>(*logarithm);
    }
    if (const NaturalLogarithm* naturalLogarithm = dynamic_cast<const NaturalLogarithm*>(&part)) {
        return std::make_unique<NaturalLogarithm>(*naturalLogarithm);
    }
    if (const Sum* sum = dynamic_cast<const Sum*>(&part)) {
        return std::make_unique<Sum>(*sum);
    }
    if (const Product* product = dynamic_cast<const Product*>(&part)) {
        return std::make_unique<Product>(*product);
    }
    if (const Min* min = dynamic_cast<const Min*>(&part)) {
        return std::make_unique<Min>(*min);
    }
    if (const Max* max = dynamic_cast<const Max*>(&part)) {
        return std::make_unique<Max>(*max);
    }

    throw CalculationError("Internal error: unsupported token in expression cloning.", ErrorType::SyntaxError);
}

std::vector<std::unique_ptr<CalculationPart>> CloneCalculationParts(const std::vector<std::unique_ptr<CalculationPart>>& parts) {
    std::vector<std::unique_ptr<CalculationPart>> cloned;
    cloned.reserve(parts.size());
    for (const std::unique_ptr<CalculationPart>& part : parts) {
        if (!part) {
            throw CalculationError("Internal error: encountered empty token while cloning expression.", ErrorType::SyntaxError);
        }
        cloned.push_back(CloneCalculationPart(*part));
    }
    return cloned;
}