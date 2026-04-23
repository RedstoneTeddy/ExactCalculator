import cpp_main
import pickle




def Save_session(calc: cpp_main.Calculator, filename: str) -> None:
    variable_dict: dict[str, str] = {}

    variable_names: list[str] = calc.Get_variable_names()
    for name in variable_names:
        calc.Calculate(name)
        variable_dict[name] = calc.Calculate_string()

    variable_dict["INT_Significance"] = str(calc.Significance)
    variable_dict["INT_FinalSignificance"] = str(calc.FinalSignificance)
    variable_dict["INT_RootSignificance"] = str(calc.RootSignificance)


    with open(filename, "wb") as f:
        pickle.dump(variable_dict, f)



def Load_session(calc: cpp_main.Calculator, filename: str) -> None:
    with open(filename, "rb") as f:
        variable_dict: dict[str, str] = pickle.load(f)

    calc.Significance = int(variable_dict.get("INT_Significance", "100"))
    calc.FinalSignificance = int(variable_dict.get("INT_FinalSignificance", "10"))
    calc.RootSignificance = int(variable_dict.get("INT_RootSignificance", "50"))

    for name, value in variable_dict.items():
        if name.startswith("INT_"):
            continue
        calc.Calculate(f"{name}={value}")

        