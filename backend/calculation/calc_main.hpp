#ifndef CALC_MAIN_HPP
#define CALC_MAIN_HPP

#include "base_structures.hpp"
#include "number.hpp"
#include "addition.hpp"
#include "subtraction.hpp"
#include "multiplication.hpp"   
#include "division.hpp"
#include "exponent.hpp"
#include "variables.hpp"

#include <vector>
#include <utility>
#include <memory>


class Calc_main {
private:
    std::vector<Number> varNumbers;
    std::vector<std::string> varNames;
public:
    Number Calculate_part(std::vector<std::unique_ptr<CalculationPart>>& calculation_parts);
    Calc_main();
};


#endif // CALC_MAIN_HPP
