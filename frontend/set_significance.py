import cpp_main



def Set_Significance(calc: cpp_main.Calculator, graphZoomInit: int) -> int:
    print("\n---- Set Significance ----")

    graphZoom: int = graphZoomInit

    def read_optional_int(prompt: str, invalid_message: str) -> int | None:
        print(prompt, end="")
        raw_value = input().strip()
        if raw_value == "":
            return None
        try:
            return int(raw_value)
        except ValueError:
            print(invalid_message)
            return None

    new_significance = read_optional_int(
        f"Internal Significance [default: 100, current: {calc.Significance}] > ",
        "Invalid input. Significance must be an integer.",
    )
    if new_significance is not None:
        calc.Significance = new_significance
    
    new_final_significance = read_optional_int(
        f"Final / Result Significance [default: 10, current: {calc.FinalSignificance}] > ",
        "Invalid input. Final Significance must be an integer.",
    )
    if new_final_significance is not None:
        calc.FinalSignificance = new_final_significance

    new_root_significance = read_optional_int(
        f"Internal Root Significance [default: 50, current: {calc.RootSignificance}] > ",
        "Invalid input. Internal Root Significance must be an integer.",
    )
    if new_root_significance is not None:
        calc.RootSignificance = new_root_significance
        if (calc.RootSignificance*2 > calc.Significance):
            print("Warning: Internal Root Significance should ideally be at most half of the Internal Significance for optimal performance.")

    new_graph_zoom = read_optional_int(
        f"Graph Exactness Level [default: 5, current: {graphZoom}] > ",
        "Invalid input. Graph Exactness Level must be an integer.",
    )
    if new_graph_zoom is not None:
        if new_graph_zoom < 1:
            print("Invalid input. Graph Exactness Level must be a positive integer.")
        else:
            graphZoom = new_graph_zoom

    print("Significance settings updated.\n")

    return graphZoom