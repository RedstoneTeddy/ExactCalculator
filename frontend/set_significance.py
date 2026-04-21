import cpp_main



def Set_Significance(calc: cpp_main.Calculator):
    print("\n---- Set Significance ----")

    print(f"Internal Significance [default: 100, current: {calc.Significance}] > ", end="")
    try:
        new_significance: int = int(input())
        calc.Significance = new_significance
    except ValueError:
        print("Invalid input. Significance must be an integer.")
    
    print(f"Final / Result Significance [default: 10, current: {calc.FinalSignificance}] > ", end="")
    try:
        new_final_significance: int = int(input())
        calc.FinalSignificance = new_final_significance
    except ValueError:
        print("Invalid input. Final Significance must be an integer.")

    print(f"Internal Root Significance [default: 50, current: {calc.RootSignificance}] > ", end="")
    try:
        new_root_significance: int = int(input())
        calc.RootSignificance = new_root_significance
        if (calc.RootSignificance*2 > calc.Significance):
            print("Warning: Internal Root Significance should ideally be at most half of the Internal Significance for optimal performance.")
    except ValueError:
        print("Invalid input. Internal Root Significance must be an integer.")

    print("Significance settings updated.\n")