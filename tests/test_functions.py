import pytest
import tests.number_gen
import cpp_main
import math

def test_fac() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateInteger(10, True)
        expected: float = a
        for i in range(1, a):
            expected *= i
        if a == 0:
            expected = 1
        calc.Calculate("Fac{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Factorial failed for {a_str}: expected {expected}, got {result}"

def test_sin() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = math.sin(a)
        calc.Calculate("Sin{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of sin({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Sine failed for sin({a_str}): expected {expected}, got {result}"

def test_cos() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = math.cos(a)
        calc.Calculate("Cos{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of cos({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Cosine failed for cos({a_str}): expected {expected}, got {result}"

def test_tan() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = math.tan(a)
        calc.Calculate("Tan{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of tan({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Tangent failed for tan({a_str}): expected {expected}, got {result}"

def test_asin() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(20):
        a_str, a = tests.number_gen.GenerateNumber(1)
        expected: float = math.asin(a)
        calc.Calculate("Asin{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of asin({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Arcsine failed for asin({a_str}): expected {expected}, got {result}"

def test_acos() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(20):
        a_str, a = tests.number_gen.GenerateNumber(1)
        expected: float = math.acos(a)
        calc.Calculate("Acos{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of acos({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Arccosine failed for acos({a_str}): expected {expected}, got {result}"

def test_atan() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(20):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = math.atan(a)
        calc.Calculate("Atan{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of atan({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Arctangent failed for atan({a_str}): expected {expected}, got {result}"

def test_ln() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(10):
        a_str, a = tests.number_gen.GenerateNumber(100, True)
        expected: float = math.log(a)
        calc.Calculate("Ln{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of ln({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Natural logarithm failed for ln({a_str}): expected {expected}, got {result}"

def test_log() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(10):
        a_str, a = tests.number_gen.GenerateNumber(100, True)
        b_str, b = tests.number_gen.GenerateNumber(10, True)
        if a <= 0 or b <= 0 or b == 1:
            continue
        expected: float = math.log(a, b)
        calc.Calculate("Log{"+b_str+", "+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of log({a_str}, {b_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Logarithm failed for log({a_str}, {b_str}): expected {expected}, got {result}"

def test_max() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateNumber()
        expected: float = max(a, b)
        calc.Calculate("Max{"+a_str+", "+b_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of max({a_str}, {b_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Maximum failed for max({a_str}, {b_str}): expected {expected}, got {result}"

def test_min() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateNumber()
        expected: float = min(a, b)
        calc.Calculate("Min{"+a_str+", "+b_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of min({a_str}, {b_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Minimum failed for min({a_str}, {b_str}): expected {expected}, got {result}"

def test_round() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = round(a)
        calc.Calculate("Round{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of round({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Rounding failed for round({a_str}): expected {expected}, got {result}"

def test_ceil() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = math.ceil(a)
        calc.Calculate("Ceil{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of ceil({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Ceiling failed for ceil({a_str}): expected {expected}, got {result}"

def test_floor() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        expected: float = math.floor(a)
        calc.Calculate("Floor{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of floor({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Floor failed for floor({a_str}): expected {expected}, got {result}"


