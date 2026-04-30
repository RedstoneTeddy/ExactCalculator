import cpp_main

from frontend.help_text import Help_functions, Help_message
from frontend.set_significance import Set_Significance
from frontend.graph_window import GraphWindow

import frontend.session_handler as session_handler

significance: int = 100
finalSignificance: int = 10
rootSignificance: int = 50
graphZoom: int = 5
calc: cpp_main.Calculator = cpp_main.Calculator(significance)
calc.FinalSignificance = finalSignificance
calc.RootSignificance = rootSignificance

GWindow : GraphWindow = GraphWindow()

print("\n---- Exact Calculator ----")
print(" - /exit, /quit: Exit the program")
print(" - /help : Show all internal commands")

print("")

while True:
    user_input: str  = input("> ")
    if user_input.replace(" ", "") == "":
        continue

    # Commands for the user interface
    if user_input in ["/exit", "/quit"]:
        break

    if user_input == "/help":
        print(Help_message())
        continue

    if user_input.startswith("/save"):
        filename: str = user_input.split(" ", 1)[1] if len(user_input.split(" ", 1)) > 1 else "session.json"
        session_handler.Save_session(calc, filename)
        print(f"Session saved to {filename}\n")
        continue

    if user_input.startswith("/load"):
        filename = user_input.split(" ", 1)[1] if len(user_input.split(" ", 1)) > 1 else "session.json"
        try:
            session_handler.Load_session(calc, filename)
            print(f"Session loaded from {filename}\n")
        except FileNotFoundError:
            print(f"File {filename} not found.\n")
        except ValueError as error:
            print(f"Invalid session file '{filename}': {error}\n")
        continue

    if user_input.startswith("/set_significance"):
        graphZoom = Set_Significance(calc, graphZoom)
        continue


    if user_input.startswith("/graph"):
        try:
            parts = user_input.split(" ", 3)
            func_parts = parts[1:]
            func: str = " ".join(func_parts[:])
            GWindow.Load_function(func, graphZoom)
            GWindow.start()
        except (IndexError, ValueError):
            print("Invalid command. Usage: graph <function> <graph_radius> <point_distance>\n")
        continue

    if user_input.startswith("/func"):
        print(Help_functions())
        continue

    if user_input.startswith("/clear"):
        print("\033[H\033[J", end="")
        continue

    if user_input.startswith("/get_all"):
        variable_names: list[str] = calc.Get_variable_names()
        for name in variable_names:
            calc.Calculate(name)
            value = calc.Calculate_scientific_string()
            print(f"{name} = {value}")
        print("")
        continue

    if user_input.startswith("/delete"):
        try:
            variable_name: str = user_input.split(" ", 1)[1]
            calc.Delete_variable(variable_name)
            print(f"Variable '{variable_name}' deleted.\n")
        except IndexError:
            print("Invalid command. Usage: delete <variable_name>\n")
        continue


    # Send calculation to the C++ backend
    calc.Calculate(user_input)
    if calc.Get_last_error() is not None and calc.Get_last_error() != "":
        errorType: str = calc.Get_last_error().split(":")[0]
        errorRest: list[str] = calc.Get_last_error().split(":")[1:]
        errorMessage: str = ":".join(errorRest)
        print(f"---- {errorType} ----")
        print(errorMessage)
        print("")
    else:
        result = calc.Calculate_scientific_string()
        print(f"= {result}\n")




