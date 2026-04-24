# Proposals to Speed Up `preprocess.py`

Currently, `preprocess.py` is called once per file by CMake. This incurs a significant overhead due to Python's startup time and Jinja2 environment initialization for each file.

## 1. Batch Processing
Instead of processing one file per invocation, the script could accept multiple source-output pairs. 
CMake's `jinja_generate` macro could be modified to collect all generation tasks and execute them in a single Python process.

**Example command:**
```bash
preprocess.py --batch-file tasks.json
```
Where `tasks.json` contains a list of `{"source": "...", "output": "...", "mode": "..."}`.

## 2. Shared Environment
By processing all files in one go, we initialize the `jinja2.Environment` only once. This is the most expensive part of the script besides startup.

## 3. Parallelism
Inside the Python script, we can use `concurrent.futures.ProcessPoolExecutor` to process templates in parallel after the environment is ready. This is more efficient than CMake-level parallelism because it avoids redundant Python startups.

## 4. Faster Template Loading
Using a `jinja2.FileSystemBytecodeCache` can speed up subsequent runs by caching compiled Jinja2 bytecodes on disk.

## 5. Grouping by Mode
Since most files are processed in the same `yio` mode, grouping them ensures we don't switch context frequently.

## 6. Pre-compilation
We could pre-compile `library.jinja` and other common fragments into the environment once.

# Binary Size Reduction Proposals

## 1. Selective Inclusion
Allow users to disable specific type handlers (e.g., `YIO_DISABLE_FLOAT`, `YIO_DISABLE_COMPLEX`) via CMake. This would remove the corresponding code from `YYIO_PRINT_FUNC_GENERIC` and prevent linking unused internal functions.

## 2. Shared Formatting Core
Currently, each float handler (`f`, `d`, `l`) might have similar logic. Consolidating the core stringification logic (like the scientific suffix handling) into a single function that takes a precision and a `long double` (or custom decimal type) could save space.

## 3. Inline Attribute Control
Strictly control `inline` and `static inline` usage. Too much inlining in `ctx.h` and `io.h` increases the binary size of every translation unit that includes them. Use `__attribute__((noinline))` for cold error paths.

## 4. Error Message Compression
Optionally allow disabling error strings (`yio_strerror` returns only codes) to save space in embedded environments.
