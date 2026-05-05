# Internal Architecture: Templating & Code Generation

Yio uses a Jinja2-based code generation pipeline to implement its type-safe `_Generic` dispatcher and maintain consistency across a high volume of C types.

---

## 1. Generation Pipeline

The build process involves transforming templates into standard C11 source code.

- **Source Templates:** Located in `src/`. These are C files (usually in `src/yio/`) containing Jinja2 directives.
- **Preprocessor (`src/preprocess.py`):** A Python script that orchestrates the Jinja2 rendering engine.
- **Context Injection:** Technical metadata (type lists, limits, backend configurations) is injected into the templates at build time.

---

## 2. Core Macros (`src/library.jinja`)

Internal development relies on standardized macros to handle repetitive logic:

- **`j_FOREACHAPPLY(list)`**: Iterates over type configurations (e.g., `j_FLOATS`) and generates specialized handlers for each.
- **`j_seq(start, end)`**: Generates numeric sequences, used primarily for generating `_BitInt` and `_Generic` slots.

---

## 3. Automated Error Management

Error codes and their corresponding descriptive strings are managed centrally via `src/geterrors.py`.

- **Mechanism:** The script parses the source for `YYIO_ERROR(ENUM, DESC)` calls.
- **Artifacts:** Generates `yio_error_genenum.h` and `yio_error_genmsg.h`.
- **Workflow:** To add an error, use the macro in code; the build system automatically updates the public header and the `yio_strerror` mapping.

