#include "number.hpp"

#include <cmath>
#include <string>

// Constructors for the number
Number::Number(int maxSignificant) {
    isNegative = false;
    digits = {0};
    exponent = 0;
    SetMaxSignificant(maxSignificant);
}

Number::Number(int maxSignificant, bool isNegative, std::vector<int> digits, int exponent) {
    SetMaxSignificant(maxSignificant);
    SetNegative(isNegative);
    SetDigits(digits);
    SetExponent(exponent);
}






// Setters
void Number::SetNumber(bool isNegative, std::vector<int> digits, int exponent) {
    SetNegative(isNegative);
    SetDigits(digits);
    SetExponent(exponent);
}

void Number::SetNegative(bool isNegative) {
    this->isNegative = isNegative;
}

void Number::SetDigits(std::vector<int> digits) {
    this->digits.clear();
    for (int i = 0; i < std::min(maxSignificant, static_cast<int>(digits.size())); i++) {
        if (digits.at(i) < 0 || digits.at(i) > 9) {
            this->digits.push_back(0);
        } else {
            this->digits.push_back(digits.at(i));
        }
    }
}

void Number::SetExponent(int exponent) {
    this->exponent = exponent;
}

void Number::SetMaxSignificant(int maxSignificant) {
    if (maxSignificant < 1) {
        maxSignificant = 1;
    }
    this->maxSignificant = maxSignificant;
    if (digits.size() > maxSignificant) {
        SetDigits(digits);
    }
}

void Number::SetFromString(std::string value) {
    // Clear the current number
    isNegative = false;
    digits.clear();
    exponent = 0;

    // Negative sign
    if (!value.empty() && value.at(0) == '-') {
        isNegative = true;
        value.erase(0, 1);
    }

    // Read in the number
    std::vector<int> newDigits;
    bool exponentSet = false;
    for (int i = 0; i < value.size(); i++) {
        if (value.at(i) == '.') {
            // Exponent equals count of digits before the decimal point minus 1.
            if (!exponentSet) {
                exponent = static_cast<int>(newDigits.size()) - 1;
                exponentSet = true;
            }
        } else if (value.at(i) >= '0' && value.at(i) <= '9') {
            newDigits.push_back(value.at(i) - '0');
        }
    }

    if (!exponentSet) {
        exponent = static_cast<int>(newDigits.size()) - 1;
    }

    if (newDigits.empty()) {
        newDigits.push_back(0);
        exponent = 0;
        isNegative = false;
    }
    SetDigits(newDigits);
}






// Getters
bool Number::GetIsNegative() {
    return isNegative;
}

std::vector<int>& Number::GetDigits() {
    return digits;
}

const std::vector<int>& Number::GetDigits() const {
    return digits;
}

int Number::GetExponent() {
    return exponent;
}

int Number::GetMaxSignificant() {
    return maxSignificant;
}








// Special Getter (for final outputs)
double Number::GetAsDouble() {
    double result = 0.0;

    for (int i = 0; i < std::min(10, static_cast<int>(digits.size())); i++) {
        result += digits.at(i) * pow(10, exponent-i);
    }

    if (isNegative) {
        result *= -1;
    }

    return result;
}


std::string Number::GetAsString() {
    std::string result = "";

    if (isNegative) {
        result += "-";
    }

    if (digits.empty()) {
        return "0";
    }

    // Scientific notation
    for (int i = 0; i < digits.size(); i++) {
        if (i == 1) {
            result += ".";
        }
        result += std::to_string(digits.at(i));
    }
    if (exponent != 0) {
        result += "*10^" + std::to_string(exponent);
    }

    return result;
}





// Correct for the significance¨
void Number::CorrectForSignificance() {
    if (digits.size() > maxSignificant) {
        SetDigits(digits);
    }
}