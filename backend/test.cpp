#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


float some_fn(float arg1, float arg2) {
    return arg1 + arg2;
}

class SomeClass {
    float multiplier;

public:
    SomeClass(float multiplier) {
        this->multiplier = multiplier;
    }
    float multiply(float input) {
        return input * multiplier;
    }

    std::vector<float> multiply_list(std::vector<float> items) {
        for (int i = 0; i < items.size(); i++) {
            items[i] = multiply(items[i]);
        }
        return items;
    }

    void set_mult(float multiplier) {
        this->multiplier = multiplier;
    }
    float get_mult() {
        return multiplier;
    }
};


PYBIND11_MODULE(test_module, handle) {
    handle.doc() = "This is a test module.";
    handle.def("some_fn_python_name", &some_fn);

    py::class_<SomeClass>(handle, "PySomeClass")
    .def(py::init<float>())
    .def_property("multiplier", &SomeClass::get_mult, &SomeClass::set_mult)
    .def("multiply", &SomeClass::multiply)
    .def("multiply_list", &SomeClass::multiply_list);
}

// If you want to release the python-global interpreter lock (GIL), in C++:
// py::gil_scoped_release release;
// py::gil_scoped_acquire acquire;



// Python Code: 
// import test_module

// print(test_module.some_fn_python_name(2.0, 3.0))

// obj = test_module.PySomeClass(2.0)
// print(obj.multiply(3.0))

// print(obj.multiply_list([1.0, 2.0, 3.0]))

// print(obj.multiplier)
// obj.multiplier = 3.0
// print(obj.multiplier)