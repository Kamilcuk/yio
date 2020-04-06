## Contributing:

Don't hesitate to create an issue here on gitlab. I'll be more then happy to talk.

To contribute, clone the repo and make a PR. I hope I'll reponse in max 3 days.

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
