cmake_minimum_required(VERSION 3.18)
project(ExactCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python REQUIRED COMPONENTS Interpreter Development)

add_subdirectory(pybind11)
pybind11_add_module(cpp_main
	backend/cpp_main.cpp
	backend/split_string.cpp
	backend/calculation/number.cpp
	backend/calculation/calc_main.cpp
	backend/calculation/compare.cpp
	backend/calculation/addition.cpp
	backend/calculation/subtraction.cpp
	backend/calculation/multiplication.cpp
)

# target_include_directories(cpp_main PRIVATE
# 	${CMAKE_SOURCE_DIR}/backend
# )

set_target_properties(cpp_main PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
	string(TOUPPER "${cfg}" cfg_upper)
	set_target_properties(cpp_main PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}"
		RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}"
	)
endforeach()


