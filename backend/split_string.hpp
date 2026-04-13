#ifndef SPLIT_STRING_HPP
#define SPLIT_STRING_HPP

#include <vector>
#include <string>
#include <memory>

#include "calculation/base_structures.hpp"
#include "calculation/number.hpp"
#include "calculation/addition.hpp"
#include "calculation/subtraction.hpp"
#include "calculation/multiplication.hpp"

std::vector<std::unique_ptr<CalculationPart>> SplitString(std::string input, int maxSignificant);
void HandleOperator(std::vector<std::unique_ptr<CalculationPart>>& parts, std::string& currentPart, int maxSignificant);


#endif // SPLIT_STRING_HPP