#ifndef CLONE_PARTS_HPP
#define CLONE_PARTS_HPP

#include <memory>
#include <vector>

#include "base_structures.hpp"

std::unique_ptr<CalculationPart> CloneCalculationPart(const CalculationPart& part);
std::vector<std::unique_ptr<CalculationPart>> CloneCalculationParts(const std::vector<std::unique_ptr<CalculationPart>>& parts);

#endif // CLONE_PARTS_HPP