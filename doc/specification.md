# Format specification

Formatting functions like `yprintf` use the format string syntax described here.

The intention is to align the format specification to be similar to
Python Format Specification Mini-Language and `fmt::format()` Format
String Syntax and `std::formatter` from `<format>` from C++20.

Format strings contain "replacement fields" surrounded by curly braces
`{` `}`. Anything that is not contained in braces is considered literal
text, which is copied unchanged to the output. If you need to include
a brace character in the literal text, it can be escaped by doubling:
`{{` and `}}`.

The grammar for a replacement field is as follows:

```
replacement_field ::=  "{" [arg_id] ["!" conversion] [":" (format_spec | chrono_format_spec)] "}"
arg_id            ::=  digit+
digit             ::=  "0"..."9"
conversion        ::=  "a"

format_spec       ::=  [[fill]align][sign]["#"]["0"][width][grouping_option][.precision]["L"][type]
fill              ::=  <any character>
align             ::=  one of "<>=^"
sign              ::=  one of "+-" or space
width             ::=  digit+
grouping_option   ::=  one of "_,"
precision         ::=  digit+
type              ::=  one of "aAbBcdeEfFgGopsxX"

chrono_format_spec ::=  [[fill]align][width]["." precision][chrono_specs]
chrono_specs       ::=  <forwarded to strftime>
```

The fill character can be any character other than `{` or `}`. The
presence of a fill character is signaled by the character following it,
which must be one of the alignment options. If the second character of
format_spec is not a valid alignment option, then it is assumed that
both the fill character and the alignment option are absent.

The meaning of the various alignment options is as follows:

| Option | Meaning                                                                                               |
| ---    | ---                                                                                                   |
| `'<'`  | Forces the field to be left-aligned within the available space (this is the default for not numbers). |
| `'>'`  | Forces the field to be right-aligned within the available space (this is the default for numbers).    |
| `'='`  | Forces the padding to be placed after the sign (if any) but before the digits.                        |
| `'^'`  | Forces the field to be centered within the available space.                                           |

Note that unless a minimum field width is defined, the field width will
always be the same size as the data to fill it, so that the alignment
option has no meaning in this case.

The sign option is only valid for number types, and can be one of the following:

| Option | Meaning                                                                                                  |
| ---    | ---                                                                                                      |
| `'+'`  | Indicates that a sign should be used for both positive as well as negative numbers.                      |
| `'-'`  | Indicates that a sign should be used only for negative numbers (this is the default behavior).           |
| space  | indicates that a leading space should be used on positive numbers, and a minus sign on negative numbers. |

The `'#'` option causes the "alternate form" to be used for the
conversion. The alternate form is defined differently for different
types. This option is only valid for integer and floating-point types.

| _type_              | Meaning                                                             |
| ---                 | ---                                                                 |
| `'b'`               | Prepend with `'0b'`                                                 |
| `'B'`               | Prepend with `'0B'`                                                 |
| `'o'`               | Prepend with `'0'`                                                  |
| `'x'`               | Prepend with `'0x'`                                                 |
| `'X'`               | Prepend with `'0X'`                                                 |
| One of `'aAeEfFgG'` | The decimal point character is written even if no digits follow it. |
| One of `'gG'`       | Additionally to above, the trailing zeros are removed.              |

The _width_ is a decimal integer defining the minimum field width. If
not specified, then the field width will be determined by the content.

Preceding the width field by a zero (`'0'`) character enables sign-aware
zero-padding for numeric types. It forces the padding to be placed
after the sign or base (if any) but before the digits. This is used for
printing fields in the form ‘+000000120’. This option is only valid
for numeric types and it has no effect on formatting of infinity and NaN.

The precision is a decimal number indicating how many digits should
be displayed after the decimal point for a floating-point value
formatted with `'f'` and `'F'`, or before and after the decimal point for
a floating-point value formatted with `'g'` or `'G'`. For non-number types
the field indicates the maximum field size - in other words, how many
characters will be used from the field content. The precision is not
allowed for integer, character, Boolean, and pointer values.

C string does __not__ have to be null-terminated if precision is specified.

The `'L'` option uses the current locale setting to insert the appropriate
number separator characters. This option is only valid for numeric types.

Finally, the _type_ determines how the data should be presented.

The available string representation types are:
- none, `s`: Copies the string to the output.

The available `char` presentation types are:
- none, `c`: Copies the character to the output.
- `b`, `B`, `d`, `o`, `x`, `X`: Uses integer presentation types.

The available `bool` presentation types are:
- none, `s`: Copies textual representation (true or false, or the locale-specific form) to the output.
  - The locale specific form are `YESSTR` or `NOSTR` from `nl_langinfo`.
- `b`, `B`, `c`, `d`, `o`, `x`, `X`: Uses integer presentation types with the value `(unsigned char)value`.

The available integer presentation types:
- `b`: Binary format. Outputs the number in base 2. Using the '#' option with this type adds the prefix "0b" to the output value.
- `B`: Same as `b`, but with `0B` prefix.
- `d`: Decimal integer. Outputs the number in base 10.
- `o`: Octal format. Outputs the number in base 8. The base prefix is `0` if the corresponding argument value is nonzero and is empty otherwise.
- `x`: Hex format. Outputs the number in base 16, using lower-case letters for the digits above 9. The base prefix is `0x`.
- `X`: same as `x`, except that it uses uppercase letters for digits above 9 and the base prefix is `0X`.
- none: same as `d`.

The available `bool` presentation types are:
- none, `s`: Copies textual representation (`true` or `false`, or the locale-specific form) to the output.
- `b`, `B`, `c`, `d`, `o`, `x`, `X`: Uses integer presentation types with the value `(unsigned)(value)`.

The available floating-point presentation types are:
- `a`, `A`, `e`, `E`, `f`, `F`, `g`, `G`: same as `printf`

For lower-case presentation types, infinity and NaN are formatted as inf and nan, respectively. For upper-case presentation types, infinity and NaN are formatted as INF and NAN, respectively.

The available `void *` presentation types are:
- none, `p`: If std::uintptr_t is defined, produces the output as if by calling std::to_chars(first, last, reinterpret_cast<std::uintptr_t>(value), 16) with the prefix 0x added to the output; otherwise, the output is implementation-defined. 

# Based on:

- Parts taken from https://fmt.dev/latest/syntax.html © Copyright 2012-present, Victor Zverovich

```
Copyright (c) 2012 - present, Victor Zverovich

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--- Optional exception to the license ---

As an exception, if, as a result of your compiling your source code, portions of this Software are embedded into a machine-executable object form of such source code, you may redistribute such embedded portions in such object form without including the above copyright and permission notices.
```

- Parts taken from https://docs.python.org/3/library/string.html#formatspec © Copyright 2001-2022, Python Software Foundation.

```
1. This LICENSE AGREEMENT is between BeOpen.com ("BeOpen"), having an office at
   160 Saratoga Avenue, Santa Clara, CA 95051, and the Individual or Organization
   ("Licensee") accessing and otherwise using this software in source or binary
   form and its associated documentation ("the Software").

2. Subject to the terms and conditions of this BeOpen Python License Agreement,
   BeOpen hereby grants Licensee a non-exclusive, royalty-free, world-wide license
   to reproduce, analyze, test, perform and/or display publicly, prepare derivative
   works, distribute, and otherwise use the Software alone or in any derivative
   version, provided, however, that the BeOpen Python License is retained in the
   Software, alone or in any derivative version prepared by Licensee.

3. BeOpen is making the Software available to Licensee on an "AS IS" basis.
   BEOPEN MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.  BY WAY OF
   EXAMPLE, BUT NOT LIMITATION, BEOPEN MAKES NO AND DISCLAIMS ANY REPRESENTATION OR
   WARRANTY OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE
   USE OF THE SOFTWARE WILL NOT INFRINGE ANY THIRD PARTY RIGHTS.

4. BEOPEN SHALL NOT BE LIABLE TO LICENSEE OR ANY OTHER USERS OF THE SOFTWARE FOR
   ANY INCIDENTAL, SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS AS A RESULT OF USING,
   MODIFYING OR DISTRIBUTING THE SOFTWARE, OR ANY DERIVATIVE THEREOF, EVEN IF
   ADVISED OF THE POSSIBILITY THEREOF.

5. This License Agreement will automatically terminate upon a material breach of
   its terms and conditions.

6. This License Agreement shall be governed by and interpreted in all respects
   by the law of the State of California, excluding conflict of law provisions.
   Nothing in this License Agreement shall be deemed to create any relationship of
   agency, partnership, or joint venture between BeOpen and Licensee.  This License
   Agreement does not grant permission to use BeOpen trademarks or trade names in a
   trademark sense to endorse or promote products or services of Licensee, or any
   third party.  As an exception, the "BeOpen Python" logos available at
   http://www.pythonlabs.com/logos.html may be used according to the permissions
   granted on that web page.

7. By copying, installing or otherwise using the software, Licensee agrees to be
   bound by the terms and conditions of this License Agreement.
```

- Parts taken from https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification and most probably from https://en.cppreference.com/w/c/io/fprintf . Licensed under http://creativecommons.org/licenses/by-sa/3.0/ .

# Written by

Kamil Cukrowski © Copyright 2022
