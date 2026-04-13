#include "multiplication.hpp"
#include "base_operation.hpp"
#include "addition.hpp"

#include "number.hpp"

#include <vector>


Number Multiplication::Calculate(Number& a, Number& b) {
    Number result(a.GetMaxSignificant());

    for (int i = 0; i < a.GetDigits().size(); i++) {
        for (int j = 0; j < b.GetDigits().size(); j++) {

            int digit = a.GetDigits()[i] * b.GetDigits()[j];
            int exponent = a.GetExponent() - i + b.GetExponent() - j;

            Number temp(result.GetMaxSignificant());
            temp.SetNegative(false);
            std::vector<int> temp_digits;
            temp_digits.push_back(digit / 10);
            temp_digits.push_back(digit % 10);
            temp.SetDigits(temp_digits);

            temp.SetExponent(exponent+1);

            result = Addition::Calculate(result, temp);
        }
    }

    result.SetNegative(a.GetIsNegative() != b.GetIsNegative());
    result.CorrectForSignificance();
    return result;
}