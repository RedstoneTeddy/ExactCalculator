import cpp_main
import time
import random
import math

start_time: float
end_time: float
a: float
b: float
result: float
calculated: float
calculation: str


calc: cpp_main.Calculator = cpp_main.Calculator(100)
tolerance: float = 1e-7


print("---- Addition ----")
start_time = time.time()
for i in range(1000):
    a = random.random()
    b = random.random()
    result = a + b
    calculation = f"{a} + {b}"
    calculated = calc.Calculate_double(calculation)
    if not math.isclose(result, calculated, rel_tol=tolerance):
        print(f"Error: {calculation} = {result}, but got {calculated}")
end_time = time.time()
print(f"Addition (1000x) test completed in {((end_time - start_time)*1000):.4f} ms")



print("---- Subtraction ----")
start_time = time.time()
for i in range(1000):
    a = random.random()
    b = random.random()
    result = a - b
    calculation = f"{a} - {b}"
    calculated = calc.Calculate_double(calculation)
    if not math.isclose(result, calculated, rel_tol=tolerance):
        print(f"Error: {calculation} = {result}, but got {calculated}")
end_time = time.time()
print(f"Subtraction (1000x) test completed in {((end_time - start_time)*1000):.4f} ms")



print("---- Multiplication ----")
start_time = time.time()
for i in range(1000):
    a = random.random()
    b = random.random()
    result = a * b
    calculation = f"{a} * {b}"
    calculated = calc.Calculate_double(calculation)
    if not math.isclose(result, calculated, rel_tol=tolerance):
        print(f"Error: {calculation} = {result}, but got {calculated}")
end_time = time.time()
print(f"Multiplication (1000x) test completed in {((end_time - start_time)*1000):.4f} ms")



print("---- Division ----")
start_time = time.time()
for i in range(1000):
    a = random.random()
    b = random.random()
    if (b == 0):
        b = 1e-10
    result = a / b
    calculation = f"{a} / {b}"
    calculated = calc.Calculate_double(calculation)
    if not math.isclose(result, calculated, rel_tol=tolerance):
        print(f"Error: {calculation} = {result}, but got {calculated}")
end_time = time.time()
print(f"Division (1000x) test completed in {((end_time - start_time)*1000):.4f} ms")


