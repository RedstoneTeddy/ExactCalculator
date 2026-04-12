#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <vector>
#include "base_structures.hpp"
#include <string>

class Number : public CalculationPart {
public:
    Number(int maxSignificant);
    Number(int maxSignificant, bool isNegative, std::vector<int> digits, int exponent);

    void SetNumber(bool isNegative, std::vector<int> digits, int exponent);
    void SetNegative(bool isNegative);
    void SetDigits(std::vector<int> digits);
    void SetExponent(int exponent);
    void SetMaxSignificant(int maxSignificant);

    void SetFromString(std::string value);

    bool GetIsNegative();
    std::vector<int> GetDigits();
    int GetExponent();
    int GetMaxSignificant();


    double GetAsDouble();

protected:
    // Numbers are stored like they would be scientifically written, one digit per entry, first digit is before the decimal point, the others are behind.
    bool isNegative;
    std::vector<int> digits; // Each digit stored
    int exponent; // Exponent of the number, so the number is digits * 10^exponent  

    int maxSignificant;
};


#endif // NUMBER_HPP