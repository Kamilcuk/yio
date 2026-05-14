/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_PRINT_ARGUMENTS_H_
#define YIO_YIO_YIO_PRINT_ARGUMENTS_H_

/**
 * Re-evaulate the arguments
 */
#define YIO_ESC(...)  __VA_ARGS__

/**
 * Only first argument
 */
#define YIO_FIRST(_1, ...)  _1

/**
 * Just 61 commas if you're wondering.
 */
#define YIO_61COMMAS  ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

/**
 * Print the arguments using a custom callback function.
 * Actually we could name it: "register" function to print those arguments.
 * void yio_callback(int callback(yio_printctx_t *), ...);
 * @param callback The callback to call.
 * @param ... Additional arguments to call.
 */
#define yio_callback(callback, ...)  ((callback, ##__VA_ARGS__),YIO_61COMMAS)

/**
 * @def YIO_IFBA62A(expr, then, else)
 * If braces and 62 or more arguments in @c expr then @c then else @c else
 */
#define YIO_IFBA62A_0(then, else)   else
#define YIO_IFBA62A_62(then, else)  then
#define YIO_IFBA62A_N({{j_seqdashcomma(1, 63)}}, N, ...) \
		YIO_IFBA62A_##N
#define YIO_IFBA62A_IN(expr) \
		YIO_IFBA62A_N(expr, 62, {{j_seqcomma(1, 63, "0")}})
#define YIO_IFBA62A(expr, then, else)  \
		YIO_IFBA62A_IN(YIO_ESC expr)(then, else)

/**
 * @def YIO_SECONDX
 * Expand, expand and extract second argument.
 */
#define YIO_SECONDX_IN(_2)         _2
#define YIO_SECONDX(_1, _2, ...)   YIO_SECONDX_IN(_2)

/**
 * @def YIO_PRECOMMAIGNORE1
 * Forward the arguments with a leading comma and ignore first argument.
 */
#define YIO_PRECOMMAIGNORE1_0(...)
#define YIO_PRECOMMAIGNORE1_2(_1, ...)  ,__VA_ARGS__
#define YIO_PRECOMMAIGNORE1_N({{j_seqdashcomma(j_MAX_ARGS)}}, N, ...) \
		YIO_PRECOMMAIGNORE1_##N
#define YIO_PRECOMMAIGNORE1(...)  \
		YIO_PRECOMMAIGNORE1_N(__VA_ARGS__, {{j_seqcomma(j_MAX_ARGS, 1, "2")}},0,0)(__VA_ARGS__)

/**
 * @def YIO_FORWARD_XFROMSECOND
 * Force at least one expansion
 */
#define YIO_FORWARD_XFROMSECOND(_1, ...)  YIO_PRECOMMAIGNORE1(__VA_ARGS__)

/**
 * @def YIO_PRECOMMAFIRST
 * Get the first argument with a leading comma.
 */
#define YIO_PRECOMMAFIRST(a, ...)  ,a


{#
/**
 * List of argumenst _1,_2,_3,.... for a macro.
 * @param I Arguments count.
 */
#}
{% macro j_yio_print_arguments_args(I) -%}
{%- if I>1 %}{{j_seq(2, I, FMT=",_{}")}}{% endif -%}
{%- endmacro %}
{#
/**
 * Extract the printing function name from arguments.
 * If argument has more then 62 braces
 *    then extract first argument, it should be the function.
 * else
 *    apply the function passed as second argument.
 * @param I Argument to apply the transformation on
 */
#}
{% macro j_yio_macros_funcs(I) -%}
	{% for J in j_one_to_n(2, I) %}{% set A = "_"+J|string %}
		YIO_IFBA62A_IN(YIO_ESC {{A}})(YIO_SECONDX, funcgen)({{A}}, YIO_FIRST YIO_FIRST {{A}}), \
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
 * arguments passed to YIO_printf() function. Each argument if has more then 62 braces,
 * then @c YIO_FORWARD_XFROMSECOND first argument is removed and rest is passed.
 * Otherwise @c YIO_PRECOMMAFIRST a leading command is added so this doesn't need a comma.
 *
 * @param I The count of arguments passed to function.
 */
#}
{% macro j_yio_macros_args(I) %}
	{% for J in j_one_to_n(2, I) %}{% set A = "_"+J|string %}
		YIO_IFBA62A_IN(YIO_ESC {{A}})(YIO_FORWARD_XFROMSECOND, YIO_PRECOMMAFIRST)({{A}}, YIO_ESC YIO_FIRST {{A}}) \
	{% endfor %}
{% endmacro %}
#line

/**
 * On __GNUC__ use @c __extention__({...})
 * Optimized to files for better include times, cause the files are big.
 */
#if defined __GNUC__ && ! defined YIO_USE_NONGNU_MACROS //&&0
#include "yio_print_arguments_gnu.h"
#elif defined(__SDCC)
#include "yio_print_arguments_sdcc.h"
#else
#include "yio_print_arguments_nongnu.h"
#endif


/**
 * @def YIO_print_arguments_N()
 * Initial overload of argument over number of arguments.
 */
#define YIO_print_arguments_N({{j_seqdashcomma(j_MAX_ARGS)}}, N, ...)  \
		YIO_print_arguments_##N
#endif // YIO_YIO_YIO_PRINT_ARGUMENTS_H_
