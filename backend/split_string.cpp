#include "split_string.hpp"

#include <vector>
#include <string>
#include <memory>

#include "calculation/base_structures.hpp"
#include "calculation/number.hpp"
#include "calculation/addition.hpp"
#include "calculation/subtraction.hpp"


std::vector<std::unique_ptr<CalculationPart>> SplitString(std::string input, int maxSignificant) {
    std::vector<std::unique_ptr<CalculationPart>> parts;

    std::string currentPart = "";

    for (char c : input) {
        if (c == '+') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Addition>());
        }
        else if (c == '-') {
            HandleOperator(parts, currentPart, maxSignificant);
            parts.push_back(std::make_unique<Subtraction>());
        }

        else {
            currentPart += c;
        }
    }

    if (!currentPart.empty()) {
        HandleOperator(parts, currentPart, maxSignificant);
    }

    return parts;
}

void HandleOperator(std::vector<std::unique_ptr<CalculationPart>>& parts, std::string& currentPart, int maxSignificant) {
    Number num(maxSignificant);
    num.SetFromString(currentPart);
    parts.push_back(std::make_unique<Number>(num));
    currentPart = "";
}