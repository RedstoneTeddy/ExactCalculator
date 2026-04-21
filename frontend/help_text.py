

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
    - sin{n}     : Sine of n (e.g., sin{Pi/2} = 1)
    - cos{n}     : Cosine of n (e.g., cos{Pi} = -1)
    - tan{n}     : Tangent of n (e.g., tan{Pi/4} = 1)
    - asin{n}    : Arc sine of n (e.g., asin{1} = Pi/2)
    - acos{n}    : Arc cosine of n (e.g., acos{0} = Pi/2)
    - atan{n}    : Arc tangent of n (e.g., atan{1} = Pi/4)
    - ln{n}      : Natural logarithm of n (e.g., ln{E} = 1)
    - log{b, n}  : Logarithm of n to base b (e.g., log{10, 100} = 2)

"""