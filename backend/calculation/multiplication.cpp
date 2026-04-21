#include "multiplication.hpp"
#include "base_operation.hpp"
#include "subtraction.hpp"
#include "addition.hpp"

#include "number.hpp"
#include "../CalculationError.hpp"
#include <vector>


Number Multiplication::Calculate(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    if (a.GetDigits().empty()) {
        throw CalculationError("Multiplication operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }
    if (b.GetDigits().empty()) {
        throw CalculationError("Multiplication operand has no digits. Internal error in number representation.", ErrorType::SyntaxError);
    }

    a.CorrectForSignificance();
    b.CorrectForSignificance();

    if ((a.GetDigits().size() == 1 && a.GetDigits()[0] == 0) ||
        (b.GetDigits().size() == 1 && b.GetDigits()[0] == 0)) {
        return result;
    }

    int aLen = static_cast<int>(a.GetDigits().size());
    int bLen = static_cast<int>(b.GetDigits().size());
    std::vector<int> productDigits(aLen + bLen, 0);

    for (int i = aLen - 1; i >= 0; i--) {
        for (int j = bLen - 1; j >= 0; j--) {
            productDigits[i + j + 1] += a.GetDigits()[i] * b.GetDigits()[j];
        }
    }

    for (int k = static_cast<int>(productDigits.size()) - 1; k > 0; k--) {
        productDigits[k - 1] += productDigits[k] / 10;
        productDigits[k] %= 10;
    }

    while (productDigits.size() > 1 && productDigits.front() == 0) {
        productDigits.erase(productDigits.begin());
    }

    int productLen = static_cast<int>(productDigits.size());
    int productExponent = a.GetExponent() + b.GetExponent() - (aLen - 1) - (bLen - 1) + (productLen - 1);

    result.SetNumber(a.GetIsNegative() != b.GetIsNegative(), productDigits, productExponent);
    result.CorrectForSignificance();
    return result;
}