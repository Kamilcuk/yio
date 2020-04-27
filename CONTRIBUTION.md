# Code generation.

Because most of the code is repetitive, the code is auto-generated.
_All_ the `.c` and `.h` files inside `src` directory are going via `m4` preprocessor.
That means that the code goes via `m4` preprocessor, then via C preprocessor.
This allows a lot of flexibility.

The `yio_config.h.in` is preprocessed by CMake to generate the library configuration.

The `config.m4.in` is also preprocessed by CMake to generate the configuration on m4 stage.

The are many function that I needed to copy into `m4/` directory. The quoting used is the 
standard one - backtick start the quoting and `'` ends it. Because of that all character
constants `'x'` should be declared separately `static const char iks = 'x';`. 
It renders __horribly__ in eclipse, because he highlights all the text in `'` quotes
and auto-fills them. I'll be more then happy to join the dark side and use backticks
with `~` for the m4 quoting.

The `m4` configuration is stored in `m4/cfg_*.m4`. The purpose is to auto-change the code
when compiling for wide strings and utf-strings as opposed to normal strings. The "portability"
symbols are prepended with `Y`, so `Ychar_constant('x')` declares a char constant `'x'` that 
will expand to `u8'x'` when utf8 is used or to `L'x'` when wide strings are used and to just 
`'x'` when byte strings are used.

The goal in macro generation is to limit the count of macro levels when expanding, 
so that as little as possible messages are spilled by the compiler in case of an error.

# Contributing.

Don't hesitate and create an issue on gitlab. I'll be more then happy to talk.

To contribute, clone the repo and make a merge request!

Be sure to follow convention. I picked `_yIO_*` names for private symbols - it's 
very hard to keep it up, but I want to, there are many private symbols and macros.
`yio_*` are for symbols users can use. There is also `y*` most common utility functions.
I hope `y*` is a not so much used namespace in user programs worldwide.

## TODO:

- Remove stringify arguments and dissallow printing them.
    - Or add a flag for adding them.
- Remove {print,scan}data_t::argsizes members in NDEBUG builds.
- Generate the YIO_{SCAN,PRINT}_ARGUMENTS macros from m4.
    - Generate most macros from m4.
- Provide printers for all types.
    - Print wchar_t and wide strings.
- Automagically check formatting width in scan callback function. 
- Provide scanners for all types.
    - Scan for all integer types.
    - Scan for all float types.
    - Scan for all _FloatN and _DecimalN types.
    - Dynamically realloc for char** types.
    - Scan for wchar_t types.
- Implement wide string functions.
    - Generate them from normal string to share code between modes.
- Document all functions
