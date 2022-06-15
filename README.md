# :notebook: Yio Iconic Output library

> A misplaced decimal point will always end up where it will do the greatest damage.
> -- unknown  :fire: :ambulance:

:star: `YIO` library is aiming to be python-like type-safe `printf` replacement for C language.


[[_TOC_]]

# dev

[doxygen documentation](https://kamcuk.gitlab.io/yio/doxygen/index.html)
[TEMPLATING.md](./doc/TEMPLATING.md]

master: ![pipeline master status badge](https://gitlab.com/kamcuk/yio/badges/master/pipeline.svg)
devel: ![pipeline dev status badge](https://gitlab.com/kamcuk/yio/badges/devel/pipeline.svg)

# Installation

## Build, compilation and CMake

### Build dependencies

- `cmake`
- `python3` and [`jinja2`](https://pypi.org/project/Jinja2/) for templating
- `bash` and `make` are used for various administration tasks
- `git`

Mostly: `apt install -y git libunistring-dev ninja-build ; pip install jinja2`.

### Compilation and installation with CMake

```
# Configure
cmake -S . -B _build
# Build
cmake --build _build --parallel
# Install
cmake --build _build --target install --prefix /usr/local/lib
```

### CMake `add_subdirectory`

Clone the repository and just do `add_subdirectory` and link with `yio` target.
Here's an example with `FetchContent`:

```
cmake_minimum_required(VERSION 3.11)
project(yio_cmake_example)
include(FetchContent)
FetchContent_Declare(
  yio
  GIT_REPOSITORY https://gitlab.com/Kamcuk/yio.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yio)
file(WRITE main.c [=[
#include <yio.h>
int main() {
  yprintf("{}\n", 123);
}
]=])
add_executable(yourtarget main.c)
target_link_libraries(yourtarget yio)
```

### CMake with `find_package` after installation

```
find_package(yio REQUIRED)
target_link_libraries(yourtarget yio::yio)
```

## Archlinux

```
yay -S yiolibc
```

# :notebook: Documentation

> Chuck Norris counted to infinity... twice.
> -- unknown :repeat:

The project aims to implement python formatting and `std::format` formatting in C.
See [python Format specification Mini-Language](https://docs.python.org/3/library/string.html#formatspec) and [cpprefrence standard format specification](https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification).


## :beginner: Basic functionality

The library exposes it's feature using headers `yio.h`, `ywio.h`, `yc16io.h` and `yuio.h`.

The heart of the library is the function `yprintf`:

```
long yes_votes = 42572654;
long no_votes = 43132495;
double percentage = (double)yes_votes / (yes_votes + no_votes);
yprint("{:-9} YES votes  {:2.2%}\n", yes_votes, percentage);
// ' 42572654 YES votes  49.67%'
```

Another way of printing is using the `yprint` which doesn't take format string and just prints all arguments.

```
int year = 2016;
const char *event = "Referendum";
yprint("Results of the ", event, " ", event, "\n");
// 'Results of the 2016 Referendum'
```

These are two main functions. Additionally, various other variations are provided:

- `ybprint`/`ybprintf` - functions that take custom output callback
- `yfprint[f]?` - functions for printing into `FILE*`
- `yaprint[f]?` - functions just like `aprintf`, for allocating memory

Additionally:

- `yreaprint[f]?` - reallocate given memory
- `yformat[f]?` - just like `yaprintf`, but only return `NULL` in case of errors
- `yreformat[f]?` - reallocate given memory, and return a pointer to it
- `ydprint[f]?` - print into a file descriptor

Next, the library comes in 4 flavors - normal or multi-byte character mode, wide character mode, UTF-16 mode and UTF-32 mode.
Normal mode has no prefix, wide has `w`, UTF-16 uses `c16` and UTF-32 uses `u`.
Below the character `π` is used to represent the prefix.
UTF-16 and UTF-32 require libunistring library.
The prefix comes after `y` character in function names. So we have:

- `char *normal    = yformatf("Hello");` for printing using normal strings
- `wchar_t *wide   = ywformatf(L"Hello")` for printing using wide strings
- `char16_t *utf16 = yc16formatf(u"Hello")` for printing using UTF-16 strings
- `char32_t *utf32 = yuformatf(U"Hello")` for printing using UTF-32 strings

To all options a `va_list` version is provided, which has letter `v` after mode prefix. For example `yuvformatf` or `yc16vprintf`.

### Features

Implemented:

- Fill and align: `<` `>` `^`
- Sign: `+` `-` ` `
- `#` and `0`
- width and precision
- `char *` and `const char *`
    - `s` specifier
- `char`, `signed char` and `unsigned char`
    - `c` specifier
- all standard integer types
    - `b` `B` `x` `X` `o` `O` `c` `d` specifiers
    - `L` locale specific integer printing
- standard floating-point types and `_Float*` interchange
  and extended floating-point types from [n2601](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2601.pdf)
  and decimal floating-point types from [n2401](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2401.pdf)
    - `g` `G` `f` `F` `a` `A` `e` `E` specifiers
    - There are four "backends" for printing floating points.
        - based on `strfrom*`
        - "custom" in-house backend that uses functions from `math.h`
        - based on `snprintf*` call
        - based on `*ryu` implementation available from https://github.com/ulfjack/ryu
    - The library will prefer to use `strfrom*` functions if they are available.
    - You can use a macro to choose the printing function.
    - Decimal floating-point
        - require `math.h` functions or `strfrom*` support (depending on chosen backend), see https://github.com/libdfp/libdfp
        - the support in major compilers is not enough right now
- `_Imaginary` and `_Complex` standard floating-point types
    - formatting specifier is ignored
    - they are just printed as `[+-]<real>[+-]<imag>i`
- `_Accum`, `_Fract`, `stdfix.h` fixed-point types
    - `x` `X` `f` `a` `A` specifiers
- `void *`
    - `p` specifier
    - the pointer is just converted to appropriate size integer type and printed as `{:#x}`
- `struct tm` time formatting
    - the formatting string is straight passed to `strftime`
- `wchar_t *`
    - `s` specifier
- `ypmon` for printing using `strfmon`
    - formatting string is just passed to `strfmon`
- Positional arguments `{1:{2}.{3}}` are implemented
- `struct timeval` and `struct timespec`
    - format string ignored
    - just printed as `[+-]<sec>.<nano/milli-sec>`
- `!a` conversion flag, that outputs a string that can be read with C compiler

### Namespaces

 - `y*` - Many "short" common public symbols, like `yprintf`, `ysnprintf`.
 - `yio_*` `ywio_*` `yc16io_*` `yuio_*` - Public internal functions, symbols for implementators of _custom modifiers_.
 - `YIO_*` - Public symbols, macros, configuration options, constants, error codes.
 - `YYIO_*` `YYWIO_*` `YYC16IO_*` `YUIO_*` - All the plethora of private symbols that is used by the library.

### Examples :heart_eyes:

See [examples](examples) and overall all in [test](test) directory.

## Custom callbacks

For a given variable a custom callback can be provided for invoking a custom function when given as a parameter to string. The function replaces the auto-deduced callback.
The are number of given provided callbacks for various things, for example to replace `%n` printf format specifier:

```
int count;
yprintf('The value of pi is approximately {:.3f}.\n{}', 3.14, ypcount(&count));
yprintf("Above expression printed {} characters.\n", count);
```

There are available, where `π` represents mode prefix (see above):

- `yπcount` - for getting the count of characters printed
- `yπptlocaltime` - calls `localtime()` on `time_t` argument and prints it like `struct tm`
- `yπptgtime` - as above, just calls `gmtime()`
- `yπpwchar` - for printing a `wchar_t` variable, and
- `yπpwstring` - for printing a `wchar_t` string

## Extending the library

The argument to a callback function is solely `yπio_printctx_t`.
Each callback function is _required_ to first call `yπio_printctx_va_arg` to "eat" the arguments and after that the callback function _has to_ call `yπio_printctx_t` and if it's argument is not zero, return it.
This is to ensure that `va_list` stack is properly managed and that "jumping" above `va_list` elements is properly done.

#### Creating a custom callback.

Callbacks are written with special `yiocb()` callback modifier function. An example can be found at [test/examples/yio_test_custom_type_slot.c](test/examples/yio_test_custom_type_slot.).

### Slots

User can provide override for custom types in the form of "slot" macro expansions. An example is available at [test/examples/yio_test_custom_type_slot.c](test/examples/yio_test_custom_type_slot.c).

# Internal info

## Archaic and references

## :cyclone: History and design choices :school:

> [You should read more history books.](https://www.youtube.com/watch?v=tiiI5UbySSw)  
> -- Captain Jean-Luc Picard :stars: :ship:

So at first there was nothing. Then I learned about [overloading macro on number of arguments](https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments) and of course abuot newest C11 addition - `_Generic`.

> If you want to succeed, double your failure rate.  
> -- Thomas J. Watson :computer:

The first design of this library had `x` in front of it and aimed for something like this:

```
xprint("How old are you?\n");
xscan(&i);
xprint("You are ", xfmt("#*.*", 5, 10, i), " years old.\n");
```

Separately, I started to create a design for `o` in front of it, that would work just like C++ alternatives:

```
oprint("How old are you", oendl());
oscan(&i);
oprint("You are ", ofmt_fill('#'), ofmt_width(10), ofmt_precision(5), i, " years old.", oendl());
```

But I didn't like that design. What's the point of creating something people don't want to use. Using `std::cout` is so much typing and that is the reason [fmtlib/fmt](https://github.com/fmtlib/fmt) get's traction and is so popular. We want something in between - something with good checking and semi-good static type-checking. We want `printf`, but we want something flexible. We want `printf`, but we want to be able to specify custom types and format specifiers, to "overload" them and affect them.

That's why [python Format String](https://docs.python.org/3/library/string.html#formatstrings) is so nice - it allows to specify that in a short form. The problem is the static type checking, we can only do _some_ of that in C, not all. I wish there would be `consteval` in C, but that would mean compiler has to do double job.

Finally I settled for:

```
yprint("How old are you?", yendl());
yscan(&i);
yprint("You are ", yiofmt("#10.5"), i, " years old.", yendl());
```

Which is basically the same as C++ version, but just adds a custom flags modifier/function `yiofmt` that transforms the string in C specification to flags. But this, this is not enough. Cause we want `printf(_("locale strings %s \n"), "string")` which are really hard to do with streams. Also, we want choose position of arguments with that.

This also allowed for custom overloads with custom functions:

```
#define yioarr(arr, count, sep) /* some _Generic magic with custom callback*/
const int arr[10];
yprint("Array is ", yioarr(arr, 5, ", "), "\n");
```

Then I wen to implementing python like syntax:

```
yprintf("How old are you?{}", yendl());
yscanf("{}", &i);
yprintf("You are {} years old.{}", yiofmt("#10.5"), i, yendl());
```

But that is not nice - some of the arguments get a `{}`, but some don't. 
Only those get a `{}` which output anything. This is confusing. 
This breaks existent practice. My C eye is telling me - the number of `{}`
doesn't match the count of arguments. Really, even
`yprintf("You are {}{} years old.{}", yiofmt("#10.5"), i, yendl());` would be more readable.

So I decided to go with python/C++ mix:

```
yprintf("You are {:#10.5} years old.{}", i, yendl());
```

No the `:#10.5` doesn't really print the number. It is parsed the same way as `yiofmt("#10.5")` - it only set's some internal flags. Even with using `#str` macro stringify operator we could even do this:

```
yprintf("{arg1} {arg2}", arg2, arg1);
```

because the names of the variables could be stringified with `#` operator and
then we would have a table we would need to look for the proper variables name. 
it would be a mess with expanding macros, but anyway. But that offers
no static type checking against the number of arguments.
Because of how I decided to pass contexts to callbacks are checked here, both approuches are possibel 

But that is bad also. Because we don't want "one central point of all possible combinations". 
Because [customizing printf](https://www.gnu.org/software/libc/manual/html_node/Customizing-Printf.html) 
is hard and it shouldn't work that way. It should be easy to provide your own formatting 
options and types. It should be easy to parse your own options. Not that way.

So let's take another look at what we want:

1. Include what you use. Your machine does not have float or the float implementation 
   is horrible? No matter, as long as you don't print a float, you don't need to compile 
   all the float stuff.
2. Easy to customize. Easy to add your own handling functions.

And I think I've got it. It works the same as above, but the formatting is not done 
by printf. The formatting is done at the handler site. And for each type you have a 
handler.

So the float should be that way:

1. The printf prints everything up until first `{`. The it extracts the string between 
   `{` and `}`, including optional two positional arguments if needed, and hands it 
   to the next handler function.
2. Each type has a handler, ex. `int` has ex. `YYIO_print_int` handler chosen with _Generic. 
   Now that handler parses two optional arguments with the format string for each type 
   _separately_.
3. Each handler has it's own logic how to handle the format string. 

#### Changes in 2022

Because it came to me that `y*scan*` functions are hard to implement and they do not bring really much
to the library, I decided to remove all `scan` functions from the library. The library named stayed.

Also the library was rewritten from M4 preprocessor to python jinja2. Jinja2 is way simpler and easier to maintain.
This increased readability and maintanability of the library.

# :family: User reviews :scroll:

> Do something with your life finally. When will you graduate?  
> -- :woman: Mom  

> You don't spend any time with me anymore. You just sit in front that computer all the time. I have enough!  
> -- :girl: gf

> ? I don't understand a thing you're saying.  
> -- :japanese_goblin: Brother

> Whoa, bro, let's get another beer with me? \<incomprehensible noises\>  
> -- :beers: friends  
> Yea, sure.  
> -- :beer: me

# :passport_control: Author and license

Written by :japanese_ogre: Kamil Cukrowski 2020  \<kamilcukrowski :postbox: gmail.com\>.

I would be happy to rephrase the license to be more like newlib, so users can use it 
in embedded targets, but I do not know really how.

```
Yio Input Output library
Copyright (C) 2020 Kamil Cukrowski <kamilcukrowski :postbox: gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```



