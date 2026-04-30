cmake_minimum_required(VERSION 3.18)
project(ExactCalculator)

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
	backend/calculation/variables.cpp
	backend/clone_parts.cpp

	backend/functions/constants.cpp
	backend/functions/factorial.cpp
	backend/functions/root.cpp
	backend/functions/trigonometric.cpp
	backend/functions/logarithmic.cpp
	backend/functions/sum.cpp
	backend/functions/product.cpp

	backend/calculation/compare.cpp
	backend/calculation/addition.cpp
	backend/calculation/subtraction.cpp
	backend/calculation/multiplication.cpp
	backend/calculation/division.cpp
	backend/calculation/exponent.cpp
	
	backend/CalculationError.hpp
	backend/functions/minmax.cpp
	backend/functions/combinatorics.cpp
	backend/functions/round.cpp
	backend/functions/boolean.cpp
	backend/functions/prime.cpp
	backend/calculation/modulo.cpp
)

# target_include_directories(cpp_main PRIVATE
# 	${CMAKE_SOURCE_DIR}/backend
# )

set_target_properties(cpp_main PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

set(MSYS_UCRT_BIN "C:/msys64/ucrt64/bin")
foreach(runtime_dll IN ITEMS
	libgcc_s_seh-1.dll
	libwinpthread-1.dll
	libstdc++-6.dll
)
	if(EXISTS "${MSYS_UCRT_BIN}/${runtime_dll}")
		add_custom_command(TARGET cpp_main POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${MSYS_UCRT_BIN}/${runtime_dll}"
				"$<TARGET_FILE_DIR:cpp_main>"
		)
	endif()
endforeach()

foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
	string(TOUPPER "${cfg}" cfg_upper)
	set_target_properties(cpp_main PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}"
		RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}"
	)
endforeach()


