/**
 * @file
 * @date 2022.09.29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio.h"

/* ------------------------------------------------------------------------- */

struct YYIO_yio_arr_s {
	size_t elemsize;
	size_t count;
	const void *arr;
	const char *sep;
	yio_printdata_t printfunc;
};
static inline int YYIO_yio_arr(yio_printctx_t *t) {
        const struct YYIO_yio_arr_s *const arr =
                yio_printctx_va_arg(t, const struct YYIO_yio_arr_s*);
        const char *const fmtbegin = t->fmt;
        while (t->fmt[0] != '}' && t->fmt[0] != '\0') t->fmt++;
        if (t->fmt[0] != '}') {
                return YYIO_ERROR(YIO_ERROR_MON_MISSING_RIGHT_BRACE, "missing '}' when parsing yio_arr format specifier");
        }
        const char *const fmtend = t->fmt;
        int err = yio_printctx_init(t);
        if (err) return err;
        const size_t fmtlen = (size_t)(fmtend - fmtbegin);
        char *const fmt = malloc(fmtlen + 3);
        if (fmt == NULL) return YIO_ERROR_ENOMEM;
        fmt[0] = '{';
        memcpy(fmt + 1, fmtbegin, fmtlen);
        fmt[fmtlen + 1] = '}';
        fmt[fmtlen + 2] = '\0';
        const char *const sep = arr->sep != NULL ? arr->sep : ", ";
        const void *it = arr->arr;
        for (size_t i = 0; i < arr->count; ++i) {
                err = yio_printctx_printf(t, fmt, yio_callback(arr->printfunc, it));
                if (err) break;
                if (i + 1 < arr->count) {
                        err = yio_printctx_printf(t, "{}", sep);
                        if (err) break;
                }
                it = (const char *)it + arr->elemsize;
        }
        free(fmt);
        return err;
}#define YYIO_yio_arr_1(ARR)       YYIO_yio_arr_3(ARR, sizeof(ARR)/sizeof((ARR)[0]), 0)
#define YYIO_yio_arr_2(ARR, SEP)  YYIO_yio_arr_3(ARR, sizeof(ARR)/sizeof((ARR)[0]), SEP)
#define YYIO_yio_arr_3(ARR, COUNT, SEP) \
		yio_callback(YYIO_yio_arr, &(const struct YYIO_yio_arr_s){ \
				.elemsize = sizeof((ARR)[0]), \
				.count = (COUNT), \
				.arr = (ARR), \
				.sep = (SEP), \
				.printfunc = YYIO_PRINT_FUNC_GENERIC((ARR)[0]),
			})
#define YYIO_yio_arr_N(_3,_2,_1,N,...)  YYIO_yio_arr_##N
/**
 * @def yio_arr
 * @brief Print an array using custom format specifier and separator.
 * The format string is used for each element.
 * @example
 *     int arr[] = {4, 3, 2, 1};
 *     yio_printf("{}", yio_arr(arr));             // -> "4, 3, 2, 1"
 *     yio_printf("{}", yarr(arr, "|"));           // -> "4|3|2|1"
 *     yio_printf("{:02x}", yarr(arr, "|"));       // -> "04|03|02|01"
 *     int *pnt = arr;
 *     yio_printf("{:02x}", yarr(pnt, 3, "|"));    // -> "04|03|02"
 */
#define yio_arr(...)  YYIO_yio_arr_N(__VA_ARGS__, 3, 2, 1)(__VA_ARGS__)
/**
 * Short versions
 */
#define yarr    yio_arr
#define yp_arr  yio_arr


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
