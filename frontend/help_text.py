

def Help_Text() -> str:
    return """
---- Exact Calculator Help ----
- Constants:
    - Pi = 3.14...         (Pi)
    - E  = 2.71...         (Euler's number)
    - G  = 6.67...* 10^-11 (Gravitational constant)
    - C  = 299792458       (Speed of light in m/s)
    - U  = 1.66...* 10^-27 (Atomic mass unit)

- Supported functions:
    - fac{n}     : Factorial of n (e.g., fac{5} = 120)
    - sqrt{n}    : Square root of n (e.g., sqrt{16} = 4)
    - root{n, x} : n-th root of x (e.g., root{3, 27} = 3)

"""