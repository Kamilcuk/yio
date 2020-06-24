# M4

This is the folder with m4 library functions.

# Code style

All code is compiled with `-P` option - all symbols are prefixed with `m4_`.

Defined functions should also stay in this convention.

Before a function a doxygen documentation followed by a C style define can be provided.
That way you can add `*.m4` files to your IDE and it will index macros, so you can
click "go to definition" of a macro to go to m4 macro definition of your C source files.

The macro `m4_TEST` is used to write tests in it. It can be used to write in it something 
that expands to an output. That output is then given in the next line that starts exactly with 
`// PASS_REGULAR_EXPRESSION ` (including exactly 1:1 spaces). The relevant parts from
`// PASS_REGULAR_EXPRESSION ` comments are extrated by a `cmake` script and the result
of running `m4_TEST` macros is compared with them. 

# Files:

### `lib.m4`

The hearth of this library that includes other files.

### `lib/`

The folder with abstract library functions. The are written with \` as the beginning of qouting
and `~` is ending of qouting. The qouting was changed, because I want to use `'` inside my C 
code for character literals. Using `~` is also simpler, because my eclipse stops highlighting
wrongly the text in m4 macros as character literals.

#### `lib/changeoute_to_tilde.m4`

Changes the qouting from \``'` to \``~`. Needs to be called before including anything else
from the library.

#### `lib/lib.m4~

Includes all library functions.

