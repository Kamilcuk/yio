# yiofstring

Python-based f-string preprocessor for C source code.

## Features

- Replaces `f"string {expression}"` with `"string {}", expression`.
- Supports format specifiers: `f"{var:02x}"` -> `"{:02x}", var`.
- Supports `=` shorthand: `f"{var=}"` -> `"var={}", var`.
- Handles nested braces (e.g., compound literals).
- Handles ternary operators in expressions.
- Supports `F"..."` prefix.

## Usage

```bash
# Using uv
uv run compiler/fstring/yiofstring.py input.c > output.c

# Direct execution
./compiler/fstring/yiofstring.py input.c

# Read from stdin
echo 'f"hello {name}"' | ./compiler/fstring/yiofstring.py -
```

## Development

```bash
cd compiler/fstring
uv run --extra dev pytest
uv run --extra dev ruff format .
uv run --extra dev basedpyright yiofstring.py
```
