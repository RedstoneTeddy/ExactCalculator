#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>

#include "number.hpp"

namespace py = pybind11;


double TestNumber(std::string input, int maxSignificant) {
    Number num(maxSignificant);
    num.SetFromString(input);
    return num.GetAsDouble();
}




PYBIND11_MODULE(cpp_main, handle) {
    handle.doc() = "This is the main module for communicating with the backend.";
    handle.def("TestNumber", &TestNumber);

    
}
