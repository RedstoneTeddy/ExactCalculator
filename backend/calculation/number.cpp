#include "number.hpp"
#include "../CalculationError.hpp"

#include <algorithm>
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

    if (value.empty()) {
        throw CalculationError("The number literal is empty.", ErrorType::SyntaxError);
    }

    // Negative sign
    if (!value.empty() && (value.at(0) == '_' || value.at(0) == '-')) {
        isNegative = true;
        value.erase(0, 1);
    }

    if (value.empty()) {
        throw CalculationError("The number literal is empty.", ErrorType::SyntaxError);
    }

    // Parse optional scientific exponent (e.g. 1.23e-4)
    int scientificExponent = 0;
    std::string mantissa = value;
    std::size_t ePos = value.find_first_of("eE");
    if (ePos != std::string::npos) {
        if (value.find_first_of("eE", ePos + 1) != std::string::npos) {
            throw CalculationError("The number literal contains more than one scientific exponent marker.", ErrorType::SyntaxError);
        }
        mantissa = value.substr(0, ePos);
        std::string exponentPart = value.substr(ePos + 1);

        if (mantissa.empty() || exponentPart.empty()) {
            throw CalculationError("The number literal is missing a mantissa or exponent.", ErrorType::SyntaxError);
        }

        int sign = 1;
        std::size_t expIndex = 0;
        if (!exponentPart.empty() && (exponentPart.at(0) == '+' || exponentPart.at(0) == '-')) {
            sign = exponentPart.at(0) == '-' ? -1 : 1;
            expIndex = 1;
        }

        if (expIndex >= exponentPart.size()) {
            throw CalculationError("The scientific exponent is missing digits.", ErrorType::SyntaxError);
        }

        int parsedExponent = 0;
        bool exponentDigitSeen = false;
        for (; expIndex < exponentPart.size(); expIndex++) {
            char c = exponentPart.at(expIndex);
            if (c >= '0' && c <= '9') {
                exponentDigitSeen = true;
                parsedExponent = parsedExponent * 10 + (c - '0');
            } else {
                throw CalculationError("The scientific exponent may only contain digits, optionally after a leading sign.", ErrorType::SyntaxError);
            }
        }
        if (!exponentDigitSeen) {
            throw CalculationError("The scientific exponent does not contain any digits.", ErrorType::SyntaxError);
        }
        scientificExponent = sign * parsedExponent;
    }

    // Read in mantissa digits and decimal point position.
    std::vector<int> newDigits;
    int digitsBeforeDecimal = 0;
    bool decimalSeen = false;
    bool digitSeen = false;

    for (int i = 0; i < mantissa.size(); i++) {
        char c = mantissa.at(i);
        if (c == '.') {
            if (decimalSeen) {
                throw CalculationError("The number literal contains more than one decimal point.", ErrorType::SyntaxError);
            }
            decimalSeen = true;
        } else if (c >= '0' && c <= '9') {
            digitSeen = true;
            newDigits.push_back(c - '0');
            if (!decimalSeen) {
                digitsBeforeDecimal++;
            }
        } else {
            throw CalculationError("The number literal contains invalid characters.", ErrorType::SyntaxError);
        }
    }

    if (!digitSeen) {
        throw CalculationError("The number literal does not contain any digits.", ErrorType::SyntaxError);
    }

    exponent = digitsBeforeDecimal - 1 + scientificExponent;

    if (newDigits.empty()) {
        newDigits.push_back(0);
        exponent = 0;
        isNegative = false;
    }
    SetDigits(newDigits);
    CorrectForSignificance();
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


std::string Number::GetAsString(bool useScientific) {
    std::string result = "";

    if (isNegative) {
        result += "-";
    }

    if (digits.empty()) {
        return "0";
    }

    // Scientific notation
    if (useScientific && (exponent > -7 && exponent < 7)) {
        for (int i = 0; i < digits.size(); i++) {
            if (i == 1) {
                result += ".";
            }
            result += std::to_string(digits.at(i));
        }
        result += "*10^" + std::to_string(exponent);
    } else {
        // Standard notation
        int currentExponent = exponent;
        if (currentExponent < 0) {
            result += "0.";
            for (int i = 0; i < -currentExponent - 1; i++) {
                result += "0";
            }
        }
        for (int i = 0; i < digits.size(); i++) {
            result += std::to_string(digits.at(i));
            if (currentExponent == 0 && i != digits.size() - 1) {
                result += ".";
            }
            currentExponent--;
        }
        while (currentExponent > 0) {
            result += "0";
            currentExponent--;
        }
    }

    return result;
}





// Correct for the significance¨
void Number::CorrectForSignificance() {
    if (digits.size() > maxSignificant) {
        SetDigits(digits);
    }

    if (digits.empty()) {
        digits = {0};
        exponent = 0;
        isNegative = false;
        return;
    }

    // Remove leading zeros in one erase to avoid repeated shifting.
    std::size_t firstNonZero = 0;
    while (firstNonZero < digits.size() && digits[firstNonZero] == 0) {
        firstNonZero++;
    }

    if (firstNonZero >= digits.size()) {
        digits = {0};
        exponent = 0;
        isNegative = false;
        return;
    }

    if (firstNonZero > 0) {
        digits.erase(digits.begin(), digits.begin() + static_cast<std::ptrdiff_t>(firstNonZero));
        exponent -= static_cast<int>(firstNonZero);
    }

    // Remove trailing zeros
    while (digits.size() > 1 && digits.at(digits.size() - 1) == 0) {
        digits.pop_back();
    }

    // If the number is zero, reset the exponent and sign
    if (digits.size() == 1 && digits.at(0) == 0) {
        exponent = 0;
        isNegative = false;
    }
}