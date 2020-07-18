/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio_template(`m4_dnl);
#pragma once

/**
 * @defgroup yio_macros
 * Macros used for defining printing function dispatch
 * @{
 */

`
/**
 * Extract the printing function name from arguments.
 * If argument has more then 62 braces
 *    then extract first argument, it should be the function.
 * else
 *    apply the function passed as second argument.
 * @param _i Argument to apply the transformation on
 * @param function_arg function that get's what printing function to choose for argument
 */
#define m4_yio_macros_funcs(_i, function_arg) ~
m4_define_function(`m4_yio_macros_funcs~,
`_yIO_IFBA62A_IN(_yIO_ESC $1)(_yIO_SECONDX, $2)($1, _yIO_FIRST _yIO_FIRST $1),~)m4_dnl;

`
/**
 * Extract the sizes of arguments after decay, so that @c va_arg can
 * potentially the size of argument poped from stack.
 * If argument has more then 62 braces, then
 *   ignore for each argument apply sizeof decay
 * otherwise
 *   apply sizeof decay and add a post comma.
 * @param _i Argument to apply the transofrmation on
 */
#define m4_yio_macros_argsizes(_i) ~
m4_define_function(`m4_yio_macros_argsizes~,
`_yIO_IFBA62A_IN(_yIO_ESC $1)(_yIO_I1FSDPC_XF2, _yIO_FIRSTSIZEOFDECAYPOSTCOMMAX)($1, _yIO_ESC _yIO_FIRST $1)~)m4_dnl;

`
/**
 * The same as m4_yio_macros_argsizes but applies sizeof on dereferenced arguments.
 * This is used in scanf to get the size of output arrays and buffers.
 * @param _i Argument to apply for
 */
#define m4_yio_macros_derefsizes(_i) ~
m4_define_function(`m4_yio_macros_derefsizes~,
`_yIO_IFBA62A_IN(_yIO_ESC $1)(_yIO_EMPTY, _yIO_FIRSTSIZEOFDEREFPOSTCOMMA)($1, _yIO_ESC _yIO_FIRST $1)~)m4_dnl;

`
/**
 * Transform one argument from the list of PRINT_ARGUMENTS arguments into
 * arguments passed to _yIO_printf() function. Each argument if has more then 62 braces,
 * then @c _yIO_FORWARD_XFROMSECOND first argument is removed and rest is passed.
 * Otherwise @c _yIO_PRECOMMAFIRST a leading command is added so this doesn't need a comma.
 * @param _i Argument to apply the transofrmation on
 */
#define m4_yio_macros_args(_i) ~
m4_define_function(`m4_yio_macros_args~,
`_yIO_IFBA62A_IN(_yIO_ESC $1)(_yIO_FORWARD_XFROMSECOND, _yIO_PRECOMMAFIRST)($1, _yIO_ESC _yIO_FIRST $1)~)m4_dnl;

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
#define _yIO_print_arguments_N(m4_seqdashcomma(1, m4_MLVLS), N, ...)  \
		_yIO_print_arguments_##N


/* _yIO_scan_arguments_N ----------------------------------------------------------------------------------------------------- */

/**
 * Initial overload of argument over number of arguments.
 */
#define _yIO_scan_arguments_N(m4_seqdashcomma(1, m4_MLVLS), N, ...)  \
		_yIO_scan_arguments_##N

~)m4_dnl; m4_config_template_yio
