/**
 * @file
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YIO_MACROS_H_
#define _yIO_YIO_YIO_MACROS_H_

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
#define _yIO_IFBA62A_N({{j_seqdashcomma(1, 70)}}, N, ...) \
		_yIO_IFBA62A_##N
#define _yIO_IFBA62A_IN(expr) \
		_yIO_IFBA62A_N(expr, {{j_seqcomma(64, 74, "62")}}, {{j_seqcomma(1, 61, "0")}})
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

#endif /* _yIO_YIO_YIO_MACROS_H_ */
