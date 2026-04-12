#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <memory>

#include "calculation/number.hpp"

#include "split_string.hpp"
#include "calculation/calc_main.hpp"

namespace py = pybind11;


double TestNumber(std::string input, int maxSignificant) {
    Number num(maxSignificant);
    num.SetFromString(input);
    return num.GetAsDouble();
}

class Calculator {
public:
    Calculator(int maxSignificant) {
        this->maxSignificant = maxSignificant;
    }

    int GetMaxSignificant() {
        return maxSignificant;
    }
    void SetMaxSignificant(int maxSignificant) {
        if (maxSignificant < 1) {
            maxSignificant = 1;
        }
        this->maxSignificant = maxSignificant;
    }

    double Calculate_double(std::string input) {
        return Calculate_num(input).GetAsDouble();
    }

    std::string Calculate_string(std::string input) {
        return Calculate_num(input).GetAsString();
    }

private:
    int maxSignificant;

    Number Calculate_num(std::string input) {
        // Remove whitespace from input
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        
        std::vector<std::unique_ptr<CalculationPart>> calculation_parts = SplitString(input, maxSignificant);
        
        Calc_main calc;
        return calc.Calculate_part(calculation_parts);
    }
};




PYBIND11_MODULE(cpp_main, handle) {
    handle.doc() = "This is the main module for communicating with the backend.";
    handle.def("TestNumber", &TestNumber);


    py::class_<Calculator>(handle, "Calculator")
    .def(py::init<int>())
    .def_property("maxSignificant", &Calculator::GetMaxSignificant, &Calculator::SetMaxSignificant)
    .def("Calculate_double", &Calculator::Calculate_double)
    .def("Calculate_string", &Calculator::Calculate_string);
}
