# F-string Preprocessor Conventions

This directory contains the Python-based f-string preprocessor for the YIO project.

## Workflow

- **Formatting:** Use `ruff format`.
- **Linting:** Use `ruff check`.
- **Type Checking:** Use `basedpyright`.
- **Testing:** Use `pytest`. All new features or bug fixes must include unit tests in `tests/`.

## Implementation Details

- The preprocessor uses a manual brace-matching loop to handle nested C constructs (like compound literals) that regular expressions cannot easily parse.
- It specifically avoids splitting ternary operators `? :` when looking for format specifiers by tracking the depth of `?`.
- `uv` inline metadata is maintained in `yiofstring.py` for easy execution.
- Project metadata and tool configurations are managed in `pyproject.toml`.
