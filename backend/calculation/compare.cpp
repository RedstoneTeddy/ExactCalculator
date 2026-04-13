#include "compare.hpp"

#include "number.hpp"

int CompareNumbers(Number& num1, Number& num2) {
    if (num1 > num2) {
        return 1;
    } else if (num1 == num2) {
        return 0;
    } else {
        return -1;
    }
}