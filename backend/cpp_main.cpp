#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <memory>

#include "calculation/number.hpp"

#include "split_string.hpp"
#include "calculation/calc_main.hpp"

#include "calculation/exponent.hpp"

#include "CalculationError.hpp"

namespace py = pybind11;


double TestNumber(std::string input, int maxSignificant) {
    Number num(maxSignificant);
    num.SetFromString(input);
    return num.GetAsDouble();
}

class Calculator {
public:
    Calculator(int maxSignificant)
        : maxSignificant(maxSignificant < 1 ? 1 : maxSignificant),
          finalSignificant(maxSignificant < 1 ? 1 : maxSignificant),
          rootSignificant((maxSignificant < 1 ? 1 : maxSignificant) / 2),
          LastResult(maxSignificant < 1 ? 1 : maxSignificant) {}


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

    void Calculate(std::string input) {
        LastError = "";
        LastResult = Calculate_num(input);
    }

    double CalculateDouble() {
        return LastResult.GetAsDouble();
    }

    std::string CalculateScientificString() {
        Number rounded = LastResult;
        rounded = Exp_Round(rounded, finalSignificant);
        return rounded.GetAsString(true);
    }

    std::string CalculateString() {
        Number rounded = LastResult;
        rounded = Exp_Round(rounded, finalSignificant);
        return rounded.GetAsString();
    }

    std::string GetLastError() {
        return LastError;
    }

    std::vector<std::string> GetVariableNames() {
        return calc.GetVariableNames();
    }

    void DeleteVariable(std::string name) {
        calc.DeleteVariable(name);
    }

private:
    int maxSignificant;
    int finalSignificant;
    int rootSignificant;
    Calc_main calc;

    Number LastResult;
    std::string LastError;

    Number Calculate_num(std::string input) {
        // Remove whitespace from input
        py::gil_scoped_release release;
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

        Number result(maxSignificant);
        
        try {
            std::vector<std::unique_ptr<CalculationPart>> calculation_parts = SplitString(input, maxSignificant, rootSignificant);
            
            result = calc.Calculate_part(calculation_parts);

            // Store the result in the variable ans
            calc.SetVariable("ans", result);

        }
        catch (const CalculationError& e) {
            std::string errorTypeStr;
            switch (e.getErrorType()) {
                case ErrorType::SyntaxError:
                    errorTypeStr = "Syntax Error";
                    break;
                case ErrorType::DivisionByZero:
                    errorTypeStr = "Division by Zero";
                    break;
                case ErrorType::VariableNotDefined:
                    errorTypeStr = "Variable Not Defined";
                    break;
                case ErrorType::DomainError:
                    errorTypeStr = "Domain Error";
                    break;
                default:
                    errorTypeStr = "Unknown Error";
            }
            LastError = errorTypeStr + ": " + e.what();
            return Number(maxSignificant);
        }

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
    .def("Calculate", &Calculator::Calculate)
    .def("Get_last_error", &Calculator::GetLastError)
    .def("Calculate_double", &Calculator::CalculateDouble)
    .def("Calculate_scientific_string", &Calculator::CalculateScientificString)
    .def("Calculate_string", &Calculator::CalculateString)
    .def("Get_variable_names", &Calculator::GetVariableNames)
    .def("Delete_variable", &Calculator::DeleteVariable);
}
