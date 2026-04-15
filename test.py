import cpp_main
import time
import random
import math

start_time: float
end_time: float

a_list: list[float]
b_list: list[float]
result_list: list[float]
calculated_list: list[float]
calculation_list: list[str]

calc: cpp_main.Calculator = cpp_main.Calculator(100)
tolerance: float = 1e-9







def GenerateComplexCalculation(amount_number_range: tuple[int, int] = (5, 15)) -> str:
    calc: str = ""
    calc += GenerateNumber() # First Number
    for _ in range(random.randint(*amount_number_range)):
        operator: str = random.choice(["+", "-", "*", "/"])
        calc += f" {operator} "

        number: str = GenerateNumber()
        if random.random() < 0.1:
            number = GenerateComplexCalculation((1, 3))

        if operator == "/":
            number = GenerateNumber()
            while number == "0.0":
                number = GenerateNumber()

        calc += number
        
    return calc

def GenerateNumber() -> str:
    number: float = random.random() * random.randint(-1000, 1000)
    number_str: str = str(number)
    if number_str.startswith("-"):
        number_str = number_str.replace("-", "_", 1)
    return number_str









print("\n#### BASIC CALCULATIONS TEST ####\n")
print("---- Addition ----")
# Prepare numbers
a_list = []
b_list = []
result_list = []
calculated_list = []
calculation_list = []
for _ in range(1000):
    a_list.append(random.random())
    b_list.append(random.random())
    result_list.append(a_list[-1] + b_list[-1])
    calculation_list.append(f"{a_list[-1]} + {b_list[-1]}")
# Calculate and measure
start_time = time.time()
for i in range(1000):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(1000):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Addition (1000x) test completed in {(end_time - start_time)*1000:.4f} ms.")



print("---- Subtraction ----")
# Prepare numbers
a_list = []
b_list = []
result_list = []
calculated_list = []
calculation_list = []
for _ in range(1000):
    a_list.append(random.random())
    b_list.append(random.random())
    result_list.append(a_list[-1] - b_list[-1])
    calculation_list.append(f"{a_list[-1]} - {b_list[-1]}")
# Calculate and measure
start_time = time.time()
for i in range(1000):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(1000):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Subtraction (1000x) test completed in {(end_time - start_time)*1000:.4f} ms.")



print("---- Multiplication ----")
# Prepare numbers
a_list = []
b_list = []
result_list = []
calculated_list = []
calculation_list = []
for _ in range(1000):
    a_list.append(random.random())
    b_list.append(random.random())
    result_list.append(a_list[-1] * b_list[-1])
    calculation_list.append(f"{a_list[-1]} * {b_list[-1]}")
# Calculate and measure
start_time = time.time()
for i in range(1000):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(1000):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Multiplication (1000x) test completed in {(end_time - start_time)*1000:.4f} ms.")


print("---- Division ----")
# Prepare numbers
a_list = []
b_list = []
result_list = []
calculated_list = []
calculation_list = []
for _ in range(1000):
    a_list.append(random.random())
    b_list.append(random.random())
    if (b_list[-1] == 0):
        b_list[-1] = 1e-10
    result_list.append(a_list[-1] / b_list[-1])
    calculation_list.append(f"{a_list[-1]} / {b_list[-1]}")
# Calculate and measure
start_time = time.time()
for i in range(1000):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(1000):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Division (1000x) test completed in {(end_time - start_time)*1000:.4f} ms.")


print("---- Simple Exponentiation ----")
# Prepare numbers
a_list = []
b_list = []
result_list = []
calculated_list = []
calculation_list = []
for _ in range(1000):
    a_list.append(random.randrange(-100, 100))
    if (a_list[-1] == 0):
        a_list[-1] = 1
    b_list.append(random.randrange(-10, 10))
    result_list.append(a_list[-1] ** b_list[-1])
    calculation_list.append(f"{str(a_list[-1]).replace('-', '_')} ^ {str(b_list[-1]).replace('-', '_')}")
# Calculate and measure
start_time = time.time()
for i in range(1000):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(1000):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Exponentiation (1000x) test completed in {(end_time - start_time)*1000:.4f} ms.")




print("---- Fixed Exponentiation Speed Test ----")
# Prepare numbers
a_list = []
b_list = []
calculated_list = []
calculation_list = []
result_list = []
for _ in range(5):
    calculation_list.extend(["50 ^ 4.851", "11 ^ _2.625", "1 ^ 7.534", "83 ^ 7.612", "97 ^ 9.209", "5 ^ 0.694", "28 ^ _7.705", "39 ^ 1.173", "82 ^ 6.658", "48 ^ _3.776",
                             "70 ^ _9.184", "1 ^ 50", "1 ^ _50", "95 ^ 4.459", "51 ^ 4.306", "61 ^ _7.875", "95 ^ 0.418", "11 ^ 9.953", "42 ^ 3.037", "28 ^ _1.907"])
    result_list.extend([174463062.21205187, 0.0018464782715793052, 1.0, 405529487841447.1, 1.9777932978228442e+18, 3.0555203676533234, 7.073711911877686e-12, 73.5053219374737, 5522969300146.294, 4.4836642890317244e-07,
                        1.1340170924707157e-17, 1.0, 1.0, 658672536.7583694, 22531866.96051884, 8.720222158999665e-15, 6.709473887550058, 23172953778.71743, 85076.20223373243, 0.001738880087399992])
# Calculate and measure
start_time = time.time()
for i in range(100):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(100):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Fixed Exponentiation (100x) speed-test completed in {(end_time - start_time)*1000:.4f} ms.")





print("---- Advanced Exponentiation ----")
# Prepare numbers
a_list = []
b_list = []
result_list = []
calculated_list = []
calculation_list = []
for _ in range(100):
    a_list.append(random.randrange(1, 100))
    b_list.append(random.randrange(-10, 10) + round(random.random(), 3))
    result_list.append(a_list[-1] ** b_list[-1])
    calculation_list.append(f"{str(a_list[-1]).replace('-', '_')} ^ {str(b_list[-1]).replace('-', '_')}")
# Calculate and measure
start_time = time.time()
for i in range(100):
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(100):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Exponentiation (100x) test completed in {(end_time - start_time)*1000:.4f} ms.")








print("\n#### COMPLEX CALCULATIONS TEST ####")
# Prepare calculations
calculation_list = []
result_list = []
calculated_list = []
for _ in range(1000):
    calculation_list.append(GenerateComplexCalculation())
    result_list.append(eval(calculation_list[-1].replace("_", "-")))
# Calculate and measure
start_time = time.time()
for i in range(1000):
    # print(calculation_list[i])
    calculated_list.append(calc.Calculate_double(calculation_list[i]))
end_time = time.time()
# Check results
for i in range(1000):
    if not math.isclose(result_list[i], calculated_list[i], rel_tol=tolerance):
        print(f"Error: {calculation_list[i]} = {result_list[i]}, but got {calculated_list[i]}")
print(f"Complex calculations (1000x) test completed in {(end_time - start_time)*1000:.4f} ms.")


print("\nAll tests completed.\n")






