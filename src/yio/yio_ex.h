/**
 * @file
 * @date 2022.09.29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YIO_YIO_YIO_EX_H_
#define YIO_YIO_YIO_EX_H_
#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------------------- */

#if 1
/**
 * @def F
 * @param str The string to parse.
 * @param args... Additional positional arguents.
 * @brief
 *
 * Parse python-like F-string and extract variable names from
 * the string "{var}" and put them after the string as an argument and
 * edit the string to have the positional number of arguments.
 * This is _not_ a C preprocessor macro, I have written
 * m4 implementation below.
 * The = format specifier is supported also.
 *
 * @example
 *
 *    int var = 1;
 *    yio_printf(F("{var=:4d}"));  // -> yio_printf("var={:4d}", var);
 */
#define F(...)  __VA_ARGS__
#else
dnl This is m4 preprocessor code
define(`_F_REGEX',
	`\(^.*?[^{]{\)\([_a-zA-Z][_a-zA-Z0-9]*\)\(.*$\)')
define(`_F_SHIFT',
	`ifelse(`$#', `1', , `, 'shift($@))')dnl
define(`_F_IN', `dnl
dnl
dnl Check for empty expression in format string - not allowed.
ifelse(regexp($1, `[^{]{}[^}]'), `-1', , `dnl
errprint(`SyntaxError: F-string: empty expression not allowed: $1
')`'dnl
m4exit(`1')`'dnl
')dnl
dnl
ifelse(regexp($1, _F_REGEX), `-1', `dnl
dnl Actually output stuff - no more regex found.
$@', `dnl
_F_IN(dnl
dnl Change the first argument, replace variable name by the argument position.
patsubst($1, _F_REGEX, `\1'eval(`$# - 1')`\3')dnl
dnl Add rest of arguments.
_F_SHIFT($@)`'dnl
dnl With the new argument extracted from string.
, patsubst($1, _F_REGEX, `\2')`'dnl
)')')
define(`F', `dnl
dnl Preprocess - extract the variable in front of = and place them in the string.
_F_IN(patsubst($1, `\([^{]\){\([_a-zA-Z][_a-zA-Z0-9]*\)=', `\1\2={\2') _F_SHIFT($@))`'dnl
')
#endif

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_EX_H_
