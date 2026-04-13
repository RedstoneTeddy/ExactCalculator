#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <vector>
#include "number.hpp"

int CompareNumbers(Number& num1, Number& num2, bool ignoreSign = false, int shiftSecond = 0);
// If num1 > num2 returns  1
// If num1 = num2 returns  0
// If num1 < num2 returns -1



#endif // COMPARE_HPP