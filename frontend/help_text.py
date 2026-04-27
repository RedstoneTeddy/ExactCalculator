
def Help_message() -> str:
    return """
Internal Commands:
 - /exit, /quit: Exit the program
 - /help : Show this help message
 - /set_significance : Opens the menu to set the maximum significant digits for calculations
 - /graph <function : str> : Graph the specified function (use variable 'x')
 - /func : See the documentation for supported functions and syntax
 - /clear : Clear the console
 - /save <file : str> : Save the current state of all variables to a JSON file
 - /load <file : str> : Load variables from a JSON file
 - /get_all : Print all variables and their values
 - /delete <variable_name : str> : Delete a variable from memory
"""



def Help_functions() -> str:
    return """
---- Exact Calculator Help ----
- Constants:
    - Pi = 3.14...         (Pi)
    - E  = 2.71...         (Euler's number)
    - G  = 6.67...* 10^-11 (Gravitational constant)
    - C  = 299792458       (Speed of light in m/s)
    - U  = 1.66...* 10^-27 (Atomic mass unit)

- Supported functions:
    - Fac{n}       : Factorial of n (e.g., Fac{5} = 120)
    - Sqrt{n}      : Square root of n (e.g., Sqrt{16} = 4)
    - Root{n, x}   : n-th root of x (e.g., Root{3, 27} = 3)
    - Sin{n}       : Sine of n (e.g., Sin{Pi/2} = 1)
    - Cos{n}       : Cosine of n (e.g., Cos{Pi} = -1)
    - Tan{n}       : Tangent of n (e.g., Tan{Pi/4} = 1)
    - Asin{n}      : Arc sine of n (e.g., Asin{1} = Pi/2)
    - Acos{n}      : Arc cosine of n (e.g., Acos{0} = Pi/2)
    - Atan{n}      : Arc tangent of n (e.g., Atan{1} = Pi/4)
    - Ln{n}        : Natural logarithm of n (e.g., Ln{E} = 1)
    - Log{b, n}    : Logarithm of n to base b (e.g., Log{10, 100} = 2)
    - Sum{a, b, f} : Summation of f from a to b (e.g., Sum{i=1, 5, i^2} = 55)
    - Prod{a, b, f}: Product of f from a to b (e.g., Prod{i=1, 5, i} = 120)

"""