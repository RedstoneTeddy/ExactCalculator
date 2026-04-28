#include "round.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/addition.hpp"



Number Round::Calculate(Number number) {
    Number result(number.GetMaxSignificant());

    result.SetNumber(number.GetIsNegative(), number.GetDigits(), number.GetExponent());
    result.CorrectForSignificance();

    Addition add;

    // Strip all decimal digits and store first decimal digit separately to check if we need to round up
    std::vector<int> integerDigits;
    int firstDecimalDigit = 0;

    for (int i = 0; i < result.GetDigits().size(); i++) {
        int digitExponent = result.GetExponent() - i;
        if (digitExponent >= 0) {
            integerDigits.push_back(result.GetDigits()[i]);
        } else if (digitExponent == -1) {
            firstDecimalDigit = result.GetDigits()[i];
        } else {
            break;
        }
    }

    result.SetDigits(integerDigits);
    result.CorrectForSignificance();

    if (firstDecimalDigit >= 5) {
        Number one(result.GetMaxSignificant());
        one.SetDigits({1});
        one.SetNegative(number.GetIsNegative());
        result = add.Calculate(result, one);
    }

    result.CorrectForSignificance();
    return result;
}

Number Ceil::Calculate(Number number) {
    Number result(number.GetMaxSignificant());
    bool wasNegative = number.GetIsNegative();

    result.SetNumber(number.GetIsNegative(), number.GetDigits(), number.GetExponent());
    result.CorrectForSignificance();

    Addition add;

    // Strip all decimal digits and store if we need to round up or not.
    std::vector<int> integerDigits;
    bool hasDecimalDigits = false;

    for (int i = 0; i < result.GetDigits().size(); i++) {
        int digitExponent = result.GetExponent() - i;
        if (digitExponent >= 0) {
            integerDigits.push_back(result.GetDigits()[i]);
        } else {
            if (result.GetDigits()[i] != 0) {
                hasDecimalDigits = true;
            }
        }
    }

    result.SetDigits(integerDigits);
    result.CorrectForSignificance();

    if (hasDecimalDigits && !wasNegative) {
        Number one(result.GetMaxSignificant());
        one.SetDigits({1});
        result = add.Calculate(result, one);
    }

    result.CorrectForSignificance();
    return result;
}

Number Floor::Calculate(Number number) {
    Number result(number.GetMaxSignificant());
    bool wasNegative = number.GetIsNegative();

    Addition add;

    result.SetNumber(number.GetIsNegative(), number.GetDigits(), number.GetExponent());
    result.CorrectForSignificance();
    

    // Strip all decimal digits 
    std::vector<int> integerDigits;

    for (int i = 0; i < result.GetDigits().size(); i++) {
        int digitExponent = result.GetExponent() - i;
        if (digitExponent >= 0) {
            integerDigits.push_back(result.GetDigits()[i]);
        }
    }

    result.SetDigits(integerDigits);
    result.CorrectForSignificance();

    if (wasNegative && number.GetDigits().size() > integerDigits.size()) {
        Number one(result.GetMaxSignificant());
        one.SetDigits({1});
        one.SetNegative(true);
        result = add.Calculate(result, one);
    }

    result.CorrectForSignificance();
    return result;
}


