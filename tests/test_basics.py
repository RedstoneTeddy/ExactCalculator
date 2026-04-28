import pytest
import tests.number_gen
import cpp_main

def test_addition() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateNumber()
        expected: float = a + b
        calc.Calculate(f"{a_str} + {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} + {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Addition failed for {a_str} + {b_str}: expected {expected}, got {result}"

def test_subtraction() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateNumber()
        expected: float = a - b
        calc.Calculate(f"{a_str} - {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} - {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Subtraction failed for {a_str} - {b_str}: expected {expected}, got {result}"

def test_multiplication() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateNumber()
        expected: float = a * b
        calc.Calculate(f"{a_str} * {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} * {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Multiplication failed for {a_str} * {b_str}: expected {expected}, got {result}"

def test_division() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateNumber()
        if b == 0:
            continue
        expected: float = a / b
        calc.Calculate(f"{a_str} / {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} / {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Division failed for {a_str} / {b_str}: expected {expected}, got {result}"

def test_integer_exponentiation() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateNumber()
        b_str, b = tests.number_gen.GenerateInteger(20)
        expected: float = a ** b
        calc.Calculate(f"{a_str} ^ {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} ^ {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Integer exponentiation failed for {a_str} ^ {b_str}: expected {expected}, got {result}"


def test_sqrt() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(100):
        a_str, a = tests.number_gen.GenerateNumber()
        if a < 0:
            continue
        expected: float = a ** 0.5
        calc.Calculate("Sqrt{"+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of sqrt({a_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Square root failed for sqrt({a_str}): expected {expected}, got {result}"


def test_root() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(10):
        a_str, a = tests.number_gen.GenerateNumber(non_negative=True)
        b_str, b = tests.number_gen.GenerateInteger(10, non_negative=True)
        if b == 0:
            continue
        expected: float = a ** (1/b)
        calc.Calculate("Root{"+b_str+", "+a_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of root({a_str}, {b_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Root failed for root({a_str}, {b_str}): expected {expected}, got {result}"

def test_advanced_exponentiation() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(10):
        a_str, a = tests.number_gen.GenerateNumber(100, True)
        b_str, b = tests.number_gen.GenerateNumber(10)
        expected: float = a ** b
        calc.Calculate(f"{a_str} ^ {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} ^ {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Advanced exponentiation failed for {a_str} ^ {b_str}: expected {expected}, got {result}"

