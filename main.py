import cpp_main

from frontend.graph import Graph_function
from frontend.help_text import Help_Text
from frontend.set_significance import Set_Significance

significance: int = 100
finalSignificance: int = 10
rootSignificance: int = 50
calc: cpp_main.Calculator = cpp_main.Calculator(significance)
calc.FinalSignificance = finalSignificance
calc.RootSignificance = rootSignificance

print("---- Exact Calculator ----")
print("Internal Commands:")
print(" - exit, quit: Exit the program")
print(" - set_significance : Opens the menu to set the maximum significant digits for calculations")
print(" - graph <function : str> <graph_radius : float> <point_distance : float>: Graph the specified function")
print(" - func : See the documentation for supported functions and syntax")
print("")

while True:
    user_input: str  = input("> ")
    if user_input in ["exit", "quit", "Exit", "Quit"]:
        break

    if user_input.startswith("set_significance"):
        Set_Significance(calc)
        continue

    if user_input.replace(" ", "") == "":
        continue

    if user_input.startswith("graph "):
        try:
            parts = user_input.split(" ", 3)
            func = parts[1]
            graph_radius = float(parts[2])
            point_distance = float(parts[3])
            time : float = Graph_function(func, graph_radius, point_distance)
            print(f"Graphing completed in {time:.2f} seconds.\n")
        except (IndexError, ValueError):
            print("Invalid command. Usage: graph <function> <graph_radius> <point_distance>\n")
        continue

    if user_input.startswith("func"):
        print(Help_Text())
        continue

    calc.Calculate(user_input)
    if calc.Get_last_error() is not None and calc.Get_last_error() != "":
        errorType: str = calc.Get_last_error().split(":")[0]
        errorRest: list[str] = calc.Get_last_error().split(":")[1:]
        errorMessage: str = ":".join(errorRest)
        print(f"---- {errorType} ----")
        print(errorMessage)
        print("")
    else:
        result = calc.Calculate_string()
        print(f"= {result}\n")

