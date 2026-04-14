import cpp_main

significance: int = 100
calc: cpp_main.Calculator = cpp_main.Calculator(significance)

print("---- Exact Calculator ----")
print("Internal Commands:")
print(" - exit, quit: Exit the program")
print(" - set_significance <number>: Set the maximum number of significant digits")
print("")

while True:
    user_input: str  = input("> ")
    if user_input in ["exit", "quit", "Exit", "Quit"]:
        break

    if user_input.startswith("set_significance "):
        try:
            new_significance: int = int(user_input.split(" ")[1])
            calc.maxSignificant = new_significance
            print(f"Maximum significant digits set to {new_significance}.\n")
        except (IndexError, ValueError):
            print("Invalid command. Usage: set_significance <number>\n")
        continue

    if user_input.replace(" ", "") == "":
        continue

    result = calc.Calculate_string(user_input)
    print(f"= {result}\n")



# -51.043538115707584 + 8.151855233272002 + 234.17018213101983 + -379.495567239582 * -101.94377503372081 / 158.92761381144842 - 143.0852973314419 - -807.1673963476454 + 715.6934136736071 + -86.27649620036978 = 1728.20412867934, but got 0.0

