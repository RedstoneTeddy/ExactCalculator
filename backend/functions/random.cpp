#include "random.hpp"

#include <random>
#include "../calculation/compare.hpp"

Number RandomInt::Calculate(Number n) {
    // Check n is a non-negative integer
    n.CorrectForSignificance();
    if (n.GetExponent() - n.GetDigits().size() + 1 < 0 || n.GetDigits()[0] == 0) {
        throw CalculationError("RandomInt : Input must be a non-negative integer.", ErrorType::DomainError);
    }
    
    std::random_device rd;
    std::mt19937 rng(rd()); 

    Number result(n.GetMaxSignificant());

    std::uniform_int_distribution<int> dist(0, 9); 

    std::uniform_int_distribution<int> resultLength(1, n.GetExponent()+1);

    int length = resultLength(rng);

    while (true) {
        std::vector<int> digits;
        for (int i = 0; i < length; i++) {
            digits.push_back(dist(rng));
        }
        result.SetDigits(digits);
        result.SetExponent(length-1);
        if (CompareNumbers(result, n) <= 0) {
            break;
        }
    }
    result.SetNegative(false);

    result.CorrectForSignificance();
    return result;
}