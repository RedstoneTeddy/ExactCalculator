import cpp_main
import json

from typing import Any




def Save_session(calc: cpp_main.Calculator, filename: str) -> None:
    variables: dict[str, str] = {}

    variable_names: list[str] = calc.Get_variable_names()
    for name in variable_names:
        calc.Calculate(name)
        variables[name] = calc.Calculate_string()

    session_data: dict[str, Any] = {
        "version": 1,
        "settings": {
            "Significance": calc.Significance,
            "FinalSignificance": calc.FinalSignificance,
            "RootSignificance": calc.RootSignificance,
        },
        "variables": variables,
    }

    with open(filename, "w", encoding="utf-8") as f:
        json.dump(session_data, f, indent=2, sort_keys=True)



def Load_session(calc: cpp_main.Calculator, filename: str) -> None:
    with open(filename, "r", encoding="utf-8") as f:
        loaded_data: Any = json.load(f)

    if not isinstance(loaded_data, dict):
        raise ValueError("Invalid session file format. Expected a JSON object.")

    settings_raw = loaded_data.get("settings", {})
    if not isinstance(settings_raw, dict):
        raise ValueError("Invalid session file format. 'settings' must be a JSON object.")

    variables_raw = loaded_data.get("variables", {})
    if not isinstance(variables_raw, dict):
        raise ValueError("Invalid session file format. 'variables' must be a JSON object.")

    def parse_setting(key: str, default: int) -> int:
        raw_value = settings_raw.get(key, default)
        try:
            return int(raw_value)
        except (TypeError, ValueError):
            return default

    calc.Significance = parse_setting("Significance", 100)
    calc.FinalSignificance = parse_setting("FinalSignificance", 10)
    calc.RootSignificance = parse_setting("RootSignificance", 50)

    for name, value in variables_raw.items():
        if not isinstance(name, str):
            raise ValueError("Invalid session variable name. Expected a string key.")

        calc.Calculate(f"{name}={value}")
        if calc.Get_last_error() != "":
            raise ValueError(f"Invalid session variable '{name}': {calc.Get_last_error()}")

        