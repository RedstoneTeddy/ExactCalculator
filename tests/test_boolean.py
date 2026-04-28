import pytest
import tests.number_gen
import cpp_main
import random

def test_comparisons1() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateInteger(10)
        b_str, b = tests.number_gen.GenerateInteger(10)
        comparison_type = random.choice(["==", "!=", "<", ">", "<=", ">="])
        expected: int = 0
        match comparison_type:
            case "==":
                expected = a == b
            case "!=":
                expected = a != b
            case "<":
                expected = a < b
            case ">":
                expected = a > b
            case "<=":
                expected = a <= b
            case ">=":
                expected = a >= b
        expected = int(expected)
        calc.Calculate(f"{a_str} {comparison_type} {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} {comparison_type} {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Comparison failed for {a_str} {comparison_type} {b_str}: expected {expected}, got {result}"

def test_comparisons2() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateNumber(10)
        b_str, b = tests.number_gen.GenerateNumber(10)
        comparison_type = random.choice(["==", "!=", "<", ">", "<=", ">="])
        expected: int = 0
        match comparison_type:
            case "==":
                expected = a == b
            case "!=":
                expected = a != b
            case "<":
                expected = a < b
            case ">":
                expected = a > b
            case "<=":
                expected = a <= b
            case ">=":
                expected = a >= b
        expected = int(expected)
        calc.Calculate(f"{a_str} {comparison_type} {b_str}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of {a_str} {comparison_type} {b_str}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Comparison failed for {a_str} {comparison_type} {b_str}: expected {expected}, got {result}"

def test_boolean_connections() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateInteger(10)
        b_str, b = tests.number_gen.GenerateInteger(10)
        c_str, c = tests.number_gen.GenerateInteger(10)
        d_str, d = tests.number_gen.GenerateInteger(10)
        
        comparison_type1 = random.choice(["==", "!=", "<", ">", "<=", ">="])
        comparison_type2 = random.choice(["==", "!=", "<", ">", "<=", ">="])
        boolean_connection = random.choice(["&&", "||"])

        expected1: int = 0
        expected2: int = 0
        match comparison_type1:
            case "==":
                expected1 = a == b
            case "!=":
                expected1 = a != b
            case "<":
                expected1 = a < b
            case ">":
                expected1 = a > b
            case "<=":
                expected1 = a <= b
            case ">=":
                expected1 = a >= b
        match comparison_type2:
            case "==":
                expected2 = c == d
            case "!=":
                expected2 = c != d
            case "<":
                expected2 = c < d
            case ">":
                expected2 = c > d
            case "<=":
                expected2 = c <= d
            case ">=":
                expected2 = c >= d
        expected: int = 0
        match boolean_connection:
            case "&&":
                expected = int(expected1 and expected2)
            case "||":
                expected = int(expected1 or expected2)
        calc.Calculate(f"({a_str} {comparison_type1} {b_str}) {boolean_connection} ({c_str} {comparison_type2} {d_str})")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of ({a_str} {comparison_type1} {b_str}) {boolean_connection} ({c_str} {comparison_type2} {d_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Boolean connection failed for ({a_str} {comparison_type1} {b_str}) {boolean_connection} ({c_str} {comparison_type2} {d_str}): expected {expected}, got {result}"

def test_boolean_negation() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateInteger(10)
        b_str, b = tests.number_gen.GenerateInteger(10)
        comparison_type = random.choice(["==", "!=", "<", ">", "<=", ">="])

        expected: int = 0
        match comparison_type:
            case "==":
                expected = a == b
            case "!=":
                expected = a != b
            case "<":
                expected = a < b
            case ">":
                expected = a > b
            case "<=":
                expected = a <= b
            case ">=":
                expected = a >= b
        expected = int(not expected)
        calc.Calculate(f"!({a_str} {comparison_type} {b_str})")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of !({a_str} {comparison_type} {b_str}): {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"Boolean negation failed for !({a_str} {comparison_type} {b_str}): expected {expected}, got {result}"


def test_if_function() -> None:
    calc: cpp_main.Calculator = cpp_main.Calculator(100)
    calc.FinalSignificance = 100
    tolerance: float = 1e-5

    for _ in range(1000):
        a_str, a = tests.number_gen.GenerateInteger(10)
        b_str, b = tests.number_gen.GenerateInteger(10)
        c_str, c = tests.number_gen.GenerateInteger(10)
        d_str, d = tests.number_gen.GenerateInteger(10)
        comparison_type = random.choice(["==", "!=", "<", ">", "<=", ">="])

        condition: bool = False
        match comparison_type:
            case "==":
                condition = a == b
            case "!=":
                condition = a != b
            case "<":
                condition = a < b
            case ">":
                condition = a > b
            case "<=":
                condition = a <= b
            case ">=":
                condition = a >= b
        expected: float = c if condition else d
        calc.Calculate("If{"+a_str+" "+comparison_type+" "+b_str+", "+c_str+", "+d_str+"}")
        if (calc.Get_last_error() is not None and calc.Get_last_error() != ""):
            pytest.fail(f"Error during calculation of If{{{a_str} {comparison_type} {b_str}, {c_str}, {d_str}}}: {calc.Get_last_error()}")
        result: float = calc.Calculate_double()
        assert result == pytest.approx(expected, rel=tolerance), f"If function failed for If{{{a_str} {comparison_type} {b_str}, {c_str}, {d_str}}}: expected {expected}, got {result}"



