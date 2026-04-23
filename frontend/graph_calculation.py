from concurrent.futures import Future, ThreadPoolExecutor, TimeoutError
import cpp_main
import time

import os



def Calculate_graphs(func: list[str], timeout: float, x_values: list[float], significance: int) -> list[list[float | None]]:
    """
    Calculate the y values for the given functions and x values with a timeout.
    Parameters:
    - func: A list of mathematical functions to calculate, as strings (e.g., ["x^2", "3*x - 5"]).
    - timeout: The maximum time allowed for each function calculation in seconds.
    - x_values: A list of x values for which to calculate the corresponding y values.
    - significance: The number of significant digits for the calculations.
    Returns:
    - A list of lists containing the y values for each function. If a function times out, its corresponding list will contain None for those x values.
    """

    # Pre-allocate the full result so threaded chunks can be written back
    # to their original x-index positions.
    result : list[list[float | None]] = [[None for _ in range(len(x_values))] for _ in range(len(func))]

    max_threads: int = 0
    try:
        cpu_report: int | None = os.cpu_count()
        if cpu_report is not None:
            max_threads = cpu_report
    except Exception:
        max_threads = 4

    if max_threads <= 0:
        max_threads = 1

    # Avoid creating many idle threads for small inputs.
    max_threads = min(max_threads, max(1, len(x_values)))

    start_time: float = 0.0
    end_time: float = 0.0

    # Split the calculations-workload into chunks
    start_time = time.time()
    chunks_x: list[list[float]] = [[] for _ in range(max_threads)]
    chunks_indices: list[list[int]] = [[] for _ in range(max_threads)]

    for i in range(len(x_values)):
        chunks_x[i % max_threads].append(x_values[i])
        chunks_indices[i % max_threads].append(i)





    # Assign the work to the threads
    future_results : list[Future[list[list[float | None]]]] = []

    with ThreadPoolExecutor(max_workers=max_threads) as executor:
        for i in range(max_threads):
            future_results.append(executor.submit(__Threaded_calculation, func, chunks_x[i], significance))

    # Collect the results with timeout handling
    for future_i in range(len(future_results)):
        future = future_results[future_i]
        try:
            thread_result = future.result(timeout=timeout)
            chunk_indices = chunks_indices[future_i]
            for func_num in range(len(func)):
                for local_idx in range(len(chunk_indices)):
                    result[func_num][chunk_indices[local_idx]] = thread_result[func_num][local_idx]
        except TimeoutError:
            # Keep None values for timed-out chunk indices (already default).
            continue

    end_time = time.time()
    print(f"Calculating the graphs with {max_threads} threads took {end_time - start_time:.2f} seconds.")
    return result






def __Threaded_calculation(func: list[str], x_values: list[float], significance: int) -> list[list[float | None]]:
    """
    Helper function to perform the actual calculations for a chunk of x values.
    Parameters:
    - func: A list of mathematical functions to calculate, as strings.
    - x_values: A list of x values for which to calculate the corresponding y values.
    - significance: The number of significant digits for the calculations.
    Returns:
    - A list of lists containing the y values for each function for the given x values.
    """

    result : list[list[float | None]] = [[] for _ in range(len(func))]

    calc : cpp_main.Calculator = cpp_main.Calculator(significance)
    for func_num in range(len(func)):
        for i in range(len(x_values)):
            calc.Calculate(f"x = {str(x_values[i]).replace('-', '_')}")
            calc.Calculate(func[func_num])
            if calc.Get_last_error() != "":
                result[func_num].append(None)
            else:
                result[func_num].append(calc.Calculate_double())

    return result