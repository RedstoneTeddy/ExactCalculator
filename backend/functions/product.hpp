#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include "../CalculationError.hpp"
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"
#include "../calculation/calc_main.hpp"

#include <vector>
#include <memory>


class Product : public CalculationPart {
public:
    Number Calculate(
        std::vector<std::unique_ptr<CalculationPart>>& from,
        std::vector<std::unique_ptr<CalculationPart>>& to,
        std::vector<std::unique_ptr<CalculationPart>>& formula,
        Calc_main& calc_main
    );
};


#endif // PRODUCT_HPP