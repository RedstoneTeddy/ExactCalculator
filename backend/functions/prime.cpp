#include "prime.hpp"
#include "../calculation/base_structures.hpp"

#include "../CalculationError.hpp"

#include "../calculation/number.hpp"
#include "../calculation/modulo.hpp"
#include "../calculation/division.hpp"
#include "../calculation/addition.hpp"
#include "../calculation/subtraction.hpp"
#include "../calculation/multiplication.hpp"
#include "../calculation/compare.hpp"

#include "combinatorics.hpp"


Number Gcd::Calculate(Number a, Number b) {
    if (a.GetDigits().size() == 1 && a.GetDigits()[0] == 0) {
        return b;
    }
    if (b.GetDigits().size() == 1 && b.GetDigits()[0] == 0) {
        return a;
    }

    // Check a and b are integers
    a.CorrectForSignificance();
    b.CorrectForSignificance();
    if (a.GetExponent() - a.GetDigits().size() + 1 < 0 || b.GetExponent() - b.GetDigits().size() + 1 < 0) {
        throw CalculationError("GCD is only defined for integers.", ErrorType::DomainError);
    }

    Abs abs;

    a = abs.Calculate(a);
    b = abs.Calculate(b);

    Number numZero(a.GetMaxSignificant());
    numZero.SetDigits({0});

    while (CompareNumbers(b, numZero) != 0) {
        Number r = Modulo().Calculate(a, b);
        a = b;
        b = r;
    }

    return a;
}


Number Lcm::Calculate(Number a, Number b) {
    if (a.GetDigits().size() == 1 && a.GetDigits()[0] == 0) {
        return Number(a.GetMaxSignificant());
    }
    if (b.GetDigits().size() == 1 && b.GetDigits()[0] == 0) {
        return Number(b.GetMaxSignificant());
    }

    // Check a and b are integers
    a.CorrectForSignificance();
    b.CorrectForSignificance();
    if (a.GetExponent() - a.GetDigits().size() + 1 < 0 || b.GetExponent() - b.GetDigits().size() + 1 < 0) {
        throw CalculationError("LCM is only defined for integers.", ErrorType::DomainError);
    }

    Abs abs;

    a = abs.Calculate(a);
    b = abs.Calculate(b);

    Number gcd = Gcd().Calculate(a, b);

    Division div;
    Multiplication mult;


    Number ab = mult.Calculate(a, b);
    return div.Calculate(ab, gcd);
}


Number Nthprime::Calculate(Number n) {
    // Check n is a positive integer
    n.CorrectForSignificance();
    if (n.GetExponent() - n.GetDigits().size() + 1 < 0 || n.GetDigits()[0] == 0) {
        throw CalculationError("Input must be a positive integer.", ErrorType::DomainError);
    }
    if (n.GetIsNegative()) {
        throw CalculationError("Input must be a positive integer.", ErrorType::DomainError);
    }
    if (n.GetDigits().size() == 1 && n.GetDigits()[0] == 1 && n.GetExponent() == 0) {
        Number num(n.GetMaxSignificant());
        num.SetFromString("2");
        return num;
    }



    // Calculate the nth prime number
    std::vector<Number> primes;
    Number num(n.GetMaxSignificant());
    num.SetFromString("2");
    primes.push_back(num);

    Number numZero(n.GetMaxSignificant());
    numZero.SetDigits({0});
    Number numOne(n.GetMaxSignificant());
    numOne.SetDigits({1});
    Number numTwo(n.GetMaxSignificant());
    numTwo.SetDigits({2});

    Addition add;
    Modulo mod;
    Subtraction sub;

    // We already have 1 prime (2), so decrement n
    n = sub.Calculate(n, numOne);

    Number current(n.GetMaxSignificant());
    current.SetFromString("3");

    


    while (true) {
        bool isPrime = true;
        for (Number before : primes) {
            Number modOut = mod.Calculate(current, before);
            if (CompareNumbers(modOut, numZero) == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(current);
            
            n = sub.Calculate(n, numOne);

            if (CompareNumbers(n, numZero) == 0) {
                return current;
            }
        }
        current = add.Calculate(current, numTwo);

    }

}


