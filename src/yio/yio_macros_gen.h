{% from "library.jinja" import j_seqdashcomma, j_seqcomma %}
/**
 * @file
 * @date 2020-05-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

/* --------------------------------------------------------------------------------------------- */

/**
 * Re-evaulate the arguments
 */
#define _yIO_ESC(...)  __VA_ARGS__

/**
 * @def _yIO_IGNORE1
 * Ignore first argument
 */
#define _yIO_IGNORE1_IN(_1,...)  __VA_ARGS__
#define _yIO_IGNORE1(...)        _yIO_IGNORE1_IN(__VA_ARGS__,0)


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
 * void yiocb(int callback(yio_scanctx_t *), ...);
 * void yiocb(int callback(yio_printctx_t *), ...);
 * @param callback The callback to call.
 * @param ... Additional arguments to call.
 */
#define yiocb(callback, ...)  ((callback, ##__VA_ARGS__),_yIO_61COMMAS)

/* -------------------------------------------------------------------------------------------- */

/**
 * @def _yIO_IFBA62A(expr, then, else)
 * If braces and 62 or more arguments in @c expr then @c then else @c else
 */
#define _yIO_IFBA62A_0(then, else)   else
#define _yIO_IFBA62A_62(then, else)  then
#define _yIO_IFBA62A_N({{j_seqdashcomma(1, 70)}}, N, ...) \
		_yIO_IFBA62A_##N
#define _yIO_IFBA62A_IN(expr) \
		_yIO_IFBA62A_N(expr, {{j_seqcomma(64, 74, 62)}}, {{j_seqcomma(1, 61, 0)}})
// m4_define(«m4_yIO_IFBA62A», «_yIO_IFBA62A_IN(_yIO_ESC $1)($2, $3)»)
#define _yIO_IFBA62A(expr, then, else) m4_yIO_IFBA62A(expr, then, else)

/* ----------------------------------------------------------------------------------------------- */

/**
 * @def _yIO_EMPTY
 */
#define _yIO_EMPTY(...)

/**
 * @def _yIO_SECOND
 * Extract second
 */
#define _yIO_SECOND(_1, _2, ...)  _2

/**
 * @def _yIO_SECONDX
 * Expand and extract second
 */
#define _yIO_SECONDX_IN2(_2, ...)   _2
#define _yIO_SECONDX_IN(_2, ...)    _yIO_SECONDX_IN2(_2)
#define _yIO_SECONDX(_1, _2, ...)   _yIO_SECONDX_IN(_2)

/**
 * @def m4_SIZEOFDECAY
 * Applies sizeof on decayed argument on m4 stage.
 */
{% macro j_SIZEOFDECAY(X) %}sizeof(((void)0),({{X}}))/*NOLINT(clang-diagnostic-sizeof-array-decay)*/{% endmacro %}
// m4_define(«m4_SIZEOFDECAY», «sizeof(((void)0),($1))/*NOLINT(clang-diagnostic-sizeof-array-decay)*/»)

/**
 * @def _yIO_SIZEOFDECAY
 * Applies sizeof on decayed argument
 */
#define _yIO_SIZEOFDECAY(a, ...)  m4_SIZEOFDECAY(a)

/**
 * @def _yIO_FIRSTSIZEOFDECAYPOSTCOMMA
 * Force one level of expansion, then
 * apply decaying sizeof to the first argument, ignore the rest.
 * And add a trailing comma.
 */
#define _yIO_FIRSTSIZEOFDECAYPOSTCOMMAX(a, ...)  _yIO_SIZEOFDECAY(a),

/**
 * @def _yIO_FIRSTSIZEOFDECAYPOSTCOMMA
 * Apply decaying sizeof to the first argument, ignore the rest.
 * And add a trailing comma.
 */
#define _yIO_FIRSTSIZEOFDEREFPOSTCOMMA(a, ...)  sizeof(*(a)),

/**
 * @def _yIO_PRECOMMAIGNORE1
 * Forward the arguments with a leading comma and ignore first argument.
 */
#define _yIO_PRECOMMAIGNORE1_0(...)
#define _yIO_PRECOMMAIGNORE1_2(_1, ...)  ,__VA_ARGS__
#define _yIO_PRECOMMAIGNORE1_N({{j_seqdashcomma(0, j_MLVLS)}}, N, ...) \
		_yIO_PRECOMMAIGNORE1_##N
#define _yIO_PRECOMMAIGNORE1(...)  \
		_yIO_PRECOMMAIGNORE1_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 1, 2)}},0,0)(__VA_ARGS__)

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

/**
 * @def _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA(...)
 * @param ... List of arguments.
 *
 * Ignore First Foreach Sizeof Decay with Post Comma
 * Applies SIZEOFDECAY for each argument except the first one
 */
#define _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA_0()
#define _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA_1(_1)
{% for I in range(2, j_MLVLS + 1) %}
#define _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA_{{X}}(_1,{{j_seqdashcomma(2, I)}}) \
	{%+ for J in range(2, I + 1) %}{{j_SIZEOFDECAY("_"+J|string)}},{% endfor +%}
{% endfor %}
#define _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA_N({{j_seqdashcomma(1, j_MLVLS)}}, N, ...) \
		_yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA_##N
#define _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA(...) \
		_yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})(__VA_ARGS__)

/**
 * @def _yIO_I1FSDPC_XF2(_1, ...)
 * @param _1 Ignored argument
 * @param ... List of arguments to expand for
 *
 * Forces expansion of arguments and ignores first argument and calls
 * @c _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA for the rest of arguments.
 */
#define _yIO_I1FSDPC_XF2(_1, ...)  _yIO_IGNORE1FOREACHSIZEOFDECAYPOSTCOMMA(__VA_ARGS__)

/**
 * @def _yIO_STRLEN(string)
 * @brief Constant expression strlen
 */
#define _yIO_STRLEN(s) (\
		!(s)[0]?0:!(s)[1]?1:\
		{% for I in range(2, 121) -%}
			!(s)[{{I}}]?{{I}}:
			{%- if I % 5 -%}
				1
			{%- else -%}
				\
			{% endif %}
		{% endfor %}
		-1)
