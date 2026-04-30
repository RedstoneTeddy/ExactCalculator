import pytest
import tests.number_gen
import cpp_main
import math

def test_gcd() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateInteger(10, True)
        b_str, b = tests.number_gen.GenerateInteger(10, True)
        expected: float = math.gcd(a, b)
        calc.Calculate(f"Gcd{{{a_str}, {b_str}}}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of Gcd{{{a_str}, {b_str}}}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"GCD failed for {a_str}, {b_str}: expected {expected}, got {result}"
          
def test_lcm() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateInteger(10, True)
        b_str, b = tests.number_gen.GenerateInteger(10, True)
        expected: float = math.lcm(a, b)
        calc.Calculate(f"Lcm{{{a_str}, {b_str}}}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of Lcm{{{a_str}, {b_str}}}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"LCM failed for {a_str}, {b_str}: expected {expected}, got {result}"


def test_nthprime() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    expected_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

    for _ in range(100):
        n_str, n = tests.number_gen.GenerateInteger(24, True)
        if (n <= 0):
            continue
        expected: float = expected_primes[n-1] 
        calc.Calculate(f"Nthprime{{{n_str}}}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of Nthprime{{{n_str}}}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Nthprime failed for {n_str}: expected {expected}, got {result}"


        


