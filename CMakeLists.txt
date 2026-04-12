cmake_minimum_required(VERSION 3.18)
project(ExactCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python REQUIRED COMPONENTS Interpreter Development)

add_subdirectory(pybind11)
pybind11_add_module(cpp_main
	backend/cpp_main.cpp
	backend/number.cpp
)

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


