/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YIO_YIO_PRINT_ARGUMENTS_H_
#define _yIO_YIO_YIO_YIO_PRINT_ARGUMENTS_H_

/**
 * Re-evaulate the arguments
 */
#define _yIO_ESC(...)  __VA_ARGS__

/**
 * Only first argument
 */
#define _yIO_FIRST(_1, ...)  _1

/**
 * Just 61 commas if you're wondering.
 */
#define _yIO_61COMMAS  ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

/**
 * Print the arguments using a custom callback function.
 * Actually we could name it: "register" function to print those arguments.
 * void yiocb(int callback(yio_printctx_t *), ...);
 * @param callback The callback to call.
 * @param ... Additional arguments to call.
 */
#define yiocb(callback, ...)  ((callback, ##__VA_ARGS__),_yIO_61COMMAS)

/**
 * @def _yIO_IFBA62A(expr, then, else)
 * If braces and 62 or more arguments in @c expr then @c then else @c else
 */
#define _yIO_IFBA62A_0(then, else)   else
#define _yIO_IFBA62A_62(then, else)  then
#define _yIO_IFBA62A_N({{j_seqdashcomma(1, 63)}}, N, ...) \
		_yIO_IFBA62A_##N
#define _yIO_IFBA62A_IN(expr) \
		_yIO_IFBA62A_N(expr, 62, {{j_seqcomma(1, 63, "0")}})
#define _yIO_IFBA62A(expr, then, else)  \
		_yIO_IFBA62A_IN(_yIO_ESC expr)(then, else)

/**
 * @def _yIO_SECONDX
 * Expand, expand and extract second argument.
 */
#define _yIO_SECONDX_IN(_2)         _2
#define _yIO_SECONDX(_1, _2, ...)   _yIO_SECONDX_IN(_2)

/**
 * @def _yIO_PRECOMMAIGNORE1
 * Forward the arguments with a leading comma and ignore first argument.
 */
#define _yIO_PRECOMMAIGNORE1_0(...)
#define _yIO_PRECOMMAIGNORE1_2(_1, ...)  ,__VA_ARGS__
#define _yIO_PRECOMMAIGNORE1_N({{j_seqdashcomma(j_MLVLS)}}, N, ...) \
		_yIO_PRECOMMAIGNORE1_##N
#define _yIO_PRECOMMAIGNORE1(...)  \
		_yIO_PRECOMMAIGNORE1_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 1, "2")}},0,0)(__VA_ARGS__)

/**
 * @def _yIO_FORWARD_XFROMSECOND
 * Force at least one expansion
 */
#define _yIO_FORWARD_XFROMSECOND(_1, ...)  _yIO_PRECOMMAIGNORE1(__VA_ARGS__)

/**
 * @def _yIO_PRECOMMAFIRST
 * Get the first argument with a leading comma.
 */
#define _yIO_PRECOMMAFIRST(a, ...)  ,a

{#
/**
 * Extract the printing function name from arguments.
 * If argument has more then 62 braces
 *    then extract first argument, it should be the function.
 * else
 *    apply the function passed as second argument.
 * @param I Argument to apply the transformation on
 * @param function_arg function that get's what printing function to choose for argument
 */
#}
{% macro j_yio_macros_funcs(I) -%}
	{% for J in j_range(2, I) %}{% set A = "_"+J|string %}
		_yIO_IFBA62A_IN(_yIO_ESC {{A}})(_yIO_SECONDX, funcgen)({{A}}, _yIO_FIRST _yIO_FIRST {{A}}), \
	{% endfor %}
{%- endmacro %}
{#
/**
 * Add formatting and additional arguments to be passed from macros.
 *
 * The first argument is the formatting string passed to va_list.
 * Then if the development build is enabled IN M4 SO BEFORE BUILDING,
 *    the second argument is the list of argument sizes.
 *    Each one corresponds to the size of real argument
 *    that will be taken from va_list.
 * Then the normal va_list of all the other argument follow.
 * This macro is shared by both print and scan functions,
 * because it's the same - just the *ctx differs.
 *
 * The size_t list uses a compund literal, there is no need to optimize
 * it to __extension__({const size_t []..}) because it shouldn't make
 * to release builds.
 *
 * Transform one argument from the list of PRINT_ARGUMENTS arguments into
 * arguments passed to _yIO_printf() function. Each argument if has more then 62 braces,
 * then @c _yIO_FORWARD_XFROMSECOND first argument is removed and rest is passed.
 * Otherwise @c _yIO_PRECOMMAFIRST a leading command is added so this doesn't need a comma.
 *
 * @param I The count of arguments passed to function.
 */
#}
{% macro j_yio_macros_args(I) %}
	{% for J in j_range(2, I) %}{% set A = "_"+J|string %}
		_yIO_IFBA62A_IN(_yIO_ESC {{A}})(_yIO_FORWARD_XFROMSECOND, _yIO_PRECOMMAFIRST)({{A}}, _yIO_ESC _yIO_FIRST {{A}}) \
	{% endfor %}
{% endmacro %}
#line

/**
 * On __GNUC__ use @c __extention__({...})
 * Optimized to files for better include times, cause the files are big.
 */
#if defined __GNUC__ && ! defined YIO_USE_NONGNU_MACROS //&&0
#include "yio_print_arguments_gnu.h"
#else
#include "yio_print_arguments_nongnu.h"
#endif


/**
 * @def _yIO_print_arguments_N()
 * Initial overload of argument over number of arguments.
 */
#define _yIO_print_arguments_N({{j_seqdashcomma(j_MLVLS)}}, N, ...)  \
		_yIO_print_arguments_##N

#endif /* _yIO_YIO_YIO_YIO_PRINT_ARGUMENTS_H_ */
