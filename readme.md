# ExactCalculator

ExactCalculator is a hybrid C++ and Python calculator focused on precise arithmetic, symbolic-style function input, and interactive graphing.

The numeric engine is implemented in C++ (via pybind11), while the command-line interface and graph window are written in Python.

## Features

- Arbitrary-precision decimal arithmetic with configurable significance levels
- Core operators: `+`, `-`, `*`, `/`, `^`
- Variables and assignments: `x = 2`, `velocity = distance/time`
- Built-in constants: `Pi`, `E`, `G`, `C`, `U`
- Built-in functions:
	- `Fac{n}`
	- `Sqrt{n}`
	- `Root{n, x}`
	- `Sin{n}`, `Cos{n}`, `Tan{n}`
	- `Asin{n}`, `Acos{n}`, `Atan{n}`
	- `Ln{n}`, `Log{b, n}`
	- `Sum{a, b, f}`
	- and more ...
- Interactive graph window (`/graph`) with pan/zoom support
    - Navigation via arrow-keys
    - Zoom in/out with Shift and Up/Down keys
- Session save/load using JSON files

## Repository Structure

- `backend/` C++ parser, numeric engine, and pybind11 module glue
- `frontend/` Python CLI helpers, graphing, and session handling
- `main.py` CLI entry point

## Requirements

- Python 3 (with development headers available to CMake)
- CMake 3.18+
- C++17 compiler
- pybind11 (included in this repository)
- Python package:
	- `pygame-ce`


## Run

```bash
python main.py
```

## CLI Commands

- `/exit`, `/quit` Exit the program
- `/help` Show internal commands
- `/func` Show supported function syntax
- `/set_significance` Configure numeric significance settings
- `/graph <function>` Plot one or more functions (use variable `x`)
- `/save <file>` Save variables and settings to a JSON session file
- `/load <file>` Load variables and settings from a JSON session file
- `/get_all` Print all stored variables
- and more...

Default session file is `session.json`.

## Example Session

```text
> a = 2
= 2

> a^3 + Root{3, 27}
= 11

> Sum{i=1, 10, i}
= 55
```

## Notes

- Variables are case-sensitive.
- Unknown uppercase identifiers are currently parsed as variables unless they match built-in function names.
- Graph input supports multiple functions separated by semicolons.

