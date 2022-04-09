/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

/**
 * @defgroup yio_macros
 * Macros used for defining printing function dispatch
 * @{
 */

{#
/**
 * Extract the printing function name from arguments.
 * If argument has more then 62 braces
 *    then extract first argument, it should be the function.
 * else
 *    apply the function passed as second argument.
 * @param _i Argument to apply the transformation on
 * @param function_arg function that get's what printing function to choose for argument
 */
#}
{% macro j_yio_macros_funcs() -%}{% call j_APPLY(*varargs) -%}
	_yIO_IFBA62A_IN(_yIO_ESC $1)(_yIO_SECONDX, $2)($1, _yIO_FIRST _yIO_FIRST $1),
{%- endcall %}{%- endmacro %}

{#
/**
 * Transform one argument from the list of PRINT_ARGUMENTS arguments into
 * arguments passed to _yIO_printf() function. Each argument if has more then 62 braces,
 * then @c _yIO_FORWARD_XFROMSECOND first argument is removed and rest is passed.
 * Otherwise @c _yIO_PRECOMMAFIRST a leading command is added so this doesn't need a comma.
 * @param _i Argument to apply the transofrmation on
 */
#}
{% macro j_yio_macros_gen_args() -%}{% call j_APPLY(*varargs) -%}
	_yIO_IFBA62A_IN(_yIO_ESC $1)(_yIO_FORWARD_XFROMSECOND, _yIO_PRECOMMAFIRST)($1, _yIO_ESC _yIO_FIRST $1)
{%- endcall %}{%- endmacro %}


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
 * @param _i The count of arguments passed to function.
 */
#}
{% macro j_yio_macros_fmt_and_args(I) %}
	fmt \
	{% if I %}
		{% for J in j_range(2, I) %}
	{{ j_yio_macros_gen_args("_"+J|string) }} \
		{% endfor %}
	{% endif %}
	/**/
{% endmacro %}
#line

/**
 * @}
 */

/**
 * On __GNUC__ use @c __extentions({...})
 * Optimized to files for better include times, cause the files are insanely big.
 */
#if defined __GNUC__ && ! defined YIO_USE_NONGNU_MACROS //&&0
#include "yio_macros_gnu.h"
#else
#include "yio_macros_nongnu.h"
#endif

/* _yIO_print_arguments_N ----------------------------------------------------------------------------------------------------------------- */

/**
 * Initial overload of argument over number of arguments.
 */
#define _yΩIO_print_arguments_N({{j_seqdashcomma(j_MLVLS)}}, N, ...)  \
		_yΩIO_print_arguments_##N

