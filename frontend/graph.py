import cpp_main
import matplotlib.pyplot as plt
import time

def Graph_function(func: str, graph_radius: float = 100, point_distance: float = 1, significance: int = 10) -> float:
    """
    Calculate the graph for the given function and display it.
    Parameters:
    - func: The mathematical function to graph, as a string (e.g., "x^2 + 3*x - 5").
    - graph_radius: The range of x values to graph (from -graph_radius to graph_radius).
    - point_distance: The distance between points on the x-axis.
    - significance: The number of significant digits for the calculations.
    Note: The function should be in terms of 'x' and can include basic arithmetic operations and parentheses.
    """


    # Prepare Calculation & Calculator
    start_time = time.time()
    x : list[float] = []
    y : list[float] = []
    for i in range(-int(graph_radius/point_distance), int(graph_radius/point_distance) + 1, 1):
        x.append(i * point_distance)

    # Calculate all y values
    calc : cpp_main.Calculator = cpp_main.Calculator(significance)
    for i in range(len(x)):
        calc.Calculate_string(f"x = {str(x[i]).replace('-', '_')}")
        y.append(calc.Calculate_double(func))

    # Display the graph
    plt.plot(x, y, marker='.')
    plt.xlim(-graph_radius, graph_radius)
    plt.ylim(-graph_radius, graph_radius)
    plt.title(f"Graph of {func}")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.grid(True)
    end_time = time.time()
    plt.show()

    return end_time - start_time
