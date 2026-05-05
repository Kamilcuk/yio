# :notebook: Yio Iconic Output Library

> A misplaced decimal point will always end up where it will do the greatest damage.
> -- unknown  :fire: :ambulance:

:star: `Yio` is a type-safe string **output-only** formatting library for C11, aiming to be a Python-like replacement for `printf`. It leverages C11 `_Generic` for seamless type dispatch and compile-time safety.

## :beginner: Basic Usage

The library exposes its features using the header `yio.h`. For a full API reference, see the **[User Manual](doc/usage.md)**.

```c
#include <yio.h>
#include <time.h>

int main(void) {
    // Standard formatting with automatic type dispatch
    yio_printf("Hello, {}! Local time: {:%Y-%m-%d %H:%M:%S}\n", 
               "World", yio_localtime(time(NULL)));

    // Type-safe streaming: No format string, no mistakes.
    yio_stream("Users: ", 42, " Load: ", 0.123, "\n");

    // Positional and nested arguments
    yio_printf("{1}: {0:{2}.{3}f}\n", 3.14159, "Value", 10, 2);
}
```

## 🚀 Installation

### From Source
```bash
# Clone with submodules (for Ryu backend)
git clone --recursive https://gitlab.com/Kamcuk/yio.git
cd yio

# Configure and Build
cmake -S . -B _build
cmake --build _build

# Generate and install a package (Recommended)
cd _build && cpack -G DEB && sudo dpkg -i yio-*.deb   # Debian/Ubuntu
cd _build && cpack -G RPM && sudo rpm -i yio-*.rpm    # Fedora/CentOS/RHEL

# Alternative: Direct installation
sudo cmake --install _build --prefix /usr/local
```

### OS Packages
**Ubuntu/Debian:**
```bash
sudo apt install -y git cmake python3 python3-jinja2 ninja-build libunistring-dev libdfp-dev
```

**Arch Linux (AUR):**
```bash
yay -S yiolibc
```

## ⚙️ CMake Integration

#### Option A: `FetchContent` (Recommended)
Directly from source without manual installation:
```cmake
include(FetchContent)
FetchContent_Declare(
  yio
  GIT_REPOSITORY https://gitlab.com/Kamcuk/yio.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yio)

target_link_libraries(your_app PRIVATE yio)
```

#### Option B: `find_package`
After installing the library or package:
```cmake
find_package(yio REQUIRED)
target_link_libraries(your_app PRIVATE yio::yio)
```

#### Option C: `add_subdirectory`
Simply clone Yio into your project:
```cmake
add_subdirectory(path/to/yio)
target_link_libraries(your_app PRIVATE yio)
```

## 🛠 Build Requirements

### Core Dependencies
- `cmake` (>= 3.11)
- `python3` and [`jinja2`](https://pypi.org/project/Jinja2/) for templating
- `git` (to fetch submodules)
- `bash` and `make` (for administrative tasks)

### Optional Dependencies
- `libunistring-dev` — Optional. Used for precise UTF-8 width calculations.
- `libdfp` — Optional. Required for `_Decimal32/64/128` floating-point support.

### Bundled Dependencies
- **[Ryu](https://github.com/ulfjack/ryu):** The Ryu floating-point backend is included as a Git submodule in `third_party/ryu`.

## 📑 Documentation Index

- **[Technical Specification](doc/specification.md)** — Formal EBNF grammar, argument resolution, and type support matrix.
- **[User Manual](doc/usage.md)** — Functional categorization (I/O, allocation, buffering) and modifiers.
- **[Developer API](doc/custom_types.md)** — Custom type integration via callbacks and the `_Generic` dispatcher.
- **[Configuration Guide](doc/configuration.md)** — Detailed overview of CMake build options and backend selection.
- **[Architecture & Templating](doc/TEMPLATING.md)** — Technical details on the Jinja2-based code generation and build system.

## ✨ Key Features

- **Type Safety:** Compile-time type detection via `_Generic`. No more `%d` vs `%ld` mismatches.
- **Modern Syntax:** Python-style `{}` placeholders with support for positional and nested arguments.
- **Technical Density:** Support for standard C types, `_BitInt(N)`, `_FloatN`, `_DecimalN`, fixed-point, and complex numbers.
- **Flexible Backends:** Pluggable output (FILE, FD, putchar) and floating-point (Ryu, strfrom, printf) backends.
- **Extensible:** Standardized mechanism for adding support for user-defined types via "Slots" and "Callbacks".
- **Multibyte Aware:** Transparent handling of `wchar_t` and UTF-8 strings.

## 📚 Examples & References
Detailed usage patterns can be found in the [examples/](examples/) and [test/](test/) directories.

```c
#include <yio.h>

void demo() {
    // Working with time
    time_t now = time(NULL);
    yio_printf("Local time: {}\n", yio_localtime(now));

    // Monetary formatting
    yio_printf("Price: {:%n}\n", yio_mon(1234.56));
}
```

## 🌀 History and Design Choices :school:

> [You should read more history books.](https://www.youtube.com/watch?v=tiiI5UbySSw)  
> -- Captain Jean-Luc Picard :stars: :ship:

The design of Yio evolved from early experiments (originally prefixed with `x` and `o`) searching for a "sweet spot" between the brevity of `printf` and the type-safety of C++ streams.

### Key Milestones:
- **Modular Dispatch:** By leveraging C11 `_Generic`, Yio shifts the formatting responsibility to the type handlers themselves. This "pay-only-for-what-you-use" architecture ensures only the code for types you actually print is linked into your binary.
- **Syntactic Evolution:** Settled on a Python-inspired syntax for its expressiveness, but maintained C-like simplicity for low-level targets.
- **M4 to Jinja2 Rewrite (2022):** The entire core was rewritten from complex M4 macros to Python Jinja2 templates, significantly improving maintainability and readability.
- **Streamlining:** Removed legacy `scan` functions in 2022 to focus exclusively on high-quality output.

## 👪 User Reviews :scroll:

> Do something with your life finally. When will you graduate?  
> -- :woman: Mom  

> You don't spend any time with me anymore. You just sit in front that computer all the time. I have enough!  
> -- :woman: wife

> ? I don't understand a thing you're saying.  
> -- :japanese_goblin: Brother

> Whoa, bro, let's get another beer with me? \<incomprehensible noises\>  
> -- :beers: friends  
> Yea, sure.  
> -- :beer: me

## ⚖️ Author and License

Written by :japanese_ogre: Kamil Cukrowski 2026  \<kamilcukrowski :postbox: gmail.com\>.

Yio is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.
