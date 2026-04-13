import cpp_main

print("---- Testing Number ----")

print(cpp_main.TestNumber("123.456", 10))
print(cpp_main.TestNumber("123.456", 6))
print(cpp_main.TestNumber("123.456", 5))
print(cpp_main.TestNumber("123.456", 4))
print(cpp_main.TestNumber("123.456", 3))
print(cpp_main.TestNumber("123.456", 2))
print(cpp_main.TestNumber("123.456", 1))

print("---- Calculations ----")

calc: cpp_main.Calculator = cpp_main.Calculator(10)

calculation: str = "3 - 4"
print(calc.Calculate_string(calculation))
