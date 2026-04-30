
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

- Other information:
    - You can use the variable 'ans' to access the result of the last calculation.

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
    - Min{a, b}    : Minimum of a and b (e.g., Min{3, 5} = 3)
    - Max{a, b}    : Maximum of a and b (e.g., Max{3, 5} = 5)
    - Perm{n, k}   : Permutations of n items taken k at a time (e.g., Perm{5, 2} = 20)
    - Choose{n, k} : Combinations of n items taken k at a time (e.g., Choose{5, 2} = 10)
    - Abs{n}       : Absolute value of n (e.g., Abs{-5} = 5)
    - Round{n}     : Round n to the nearest integer (e.g., Round{3.6} = 4)
    - Ceil{n}      : Round n up to the nearest integer (e.g., Ceil{3.2} = 4)
    - Floor{n}     : Round n down to the nearest integer (e.g., Floor{3.8} = 3)
    - If{cond, true_expr, false_expr} : Evaluates cond; if true, returns true_expr, else returns false_expr (e.g., If{(1 > 0) && (1 != 2), 42, 10} = 42)
    - Gcd{a, b}    : Greatest common divisor of a and b (e.g., Gcd{48, 18} = 6)
    - Lcm{a, b}    : Least common multiple of a and b (e.g., Lcm{48, 18} = 144)
    - Nthprime{n}  : The n-th prime number (e.g., Nthprime{1} = 2, Nthprime{2} = 3, Nthprime{3} = 5, etc.)
    
"""