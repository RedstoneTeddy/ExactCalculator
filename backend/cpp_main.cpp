#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <memory>

#include "calculation/number.hpp"

#include "split_string.hpp"
#include "calculation/calc_main.hpp"

#include "calculation/exponent.hpp"

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
        this->finalSignificant = maxSignificant;
        this->rootSignificant = int(maxSignificant / 2);
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

    int GetFinalSignificant() {
        return finalSignificant;
    }
    void SetFinalSignificant(int finalSignificant) {
        if (finalSignificant < 1) {
            finalSignificant = 1;
        }
        this->finalSignificant = finalSignificant;
    }

    int GetRootSignificant() {
        return rootSignificant;
    }
    void SetRootSignificant(int rootSignificant) {
        if (rootSignificant < 1) {
            rootSignificant = 1;
        }
        this->rootSignificant = rootSignificant;
    }

    double Calculate_double(std::string input) {
        return Calculate_num(input).GetAsDouble();
    }

    std::string Calculate_string(std::string input) {
        return Calculate_num(input).GetAsString();
    }

private:
    int maxSignificant;
    int finalSignificant;
    int rootSignificant;
    Calc_main calc;

    Number Calculate_num(std::string input) {
        // Remove whitespace from input
        py::gil_scoped_release release;
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        
        std::vector<std::unique_ptr<CalculationPart>> calculation_parts = SplitString(input, maxSignificant, rootSignificant);
        
        Number result = calc.Calculate_part(calculation_parts);

        result = Exp_Round(result, finalSignificant);

        py::gil_scoped_acquire acquire;
        return result;
    }
};




PYBIND11_MODULE(cpp_main, handle) {
    handle.doc() = "This is the main module for communicating with the backend.";
    handle.def("TestNumber", &TestNumber);


    py::class_<Calculator>(handle, "Calculator")
    .def(py::init<int>())
    .def_property("Significance", &Calculator::GetMaxSignificant, &Calculator::SetMaxSignificant)
    .def_property("FinalSignificance", &Calculator::GetFinalSignificant, &Calculator::SetFinalSignificant)
    .def_property("RootSignificance", &Calculator::GetRootSignificant, &Calculator::SetRootSignificant)
    .def("Calculate_double", &Calculator::Calculate_double)
    .def("Calculate_string", &Calculator::Calculate_string);
}
