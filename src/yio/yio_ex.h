/**
 * @file
 * @date 2022.09.29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio.h"

/* ------------------------------------------------------------------------- */

struct YYIO_yp_arr_s {
	size_t elemsize;
	size_t count;
	const void *arr;
	const char *sep;
	yπio_printdata_t printfunc;
};
static inline int YYIO_yp_arr(yio_printctx_t *t) {
	const struct YYIO_yp_arr_s *const arr =
		yio_printctx_va_arg(ctx, const struct YYIO_yp_arr_s*);
	const char *const fmtbegin = t->fmt;
	while (t->fmt[0] != '}' && t->fmt[0] != '\0') t->fmt++;
	if (t->fmt[0] != '}') {
		return YYIO_ERROR(YIO_ERROR_MON_MISSING_RIGHT_BRACE, "missing '}' when parsing yp_arr format specifier");
	}
	const char *const fmtend = t->fmt;
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *const pf = yπio_printctx_get_fmt(t);
	const size_t fmtlen = fmtend - fmtbegin;
	char *const fmt = malloc(fmtlen * sizeof(char));
	memcpy(fmt, fmtbegin, fmtlen);
	const char *const sep = arr->sep != NULL ? arr->sep : ", ";
	while (count--) {
		err = yio_printctx_printf(t, fmt, yiocb(arr->printfunc, arr->arr));
		if (err) return err;
		if (count > 1) {
			err = yio_printctx_printf(t, "{}", sep);
			if (err) return err;
		}
		arr->arr = (const char *)arr->arr + arr->elemsize;
	}
	return err;
}
#define YYIO_yp_arr_1(ARR)       YYIO_yp_arr_3(ARR, sizeof(ARR)/sizeof((ARR)[0]), 0)
#define YYIO_yp_arr_2(ARR, SEP)  YYIO_yp_arr_3(ARR, sizeof(ARR)/sizeof((ARR)[0]), SEP)
#define YYIO_yp_arr_3(ARR, COUNT, SEP) \
		yiocb(YYIO_yp_arr, &(const struct YYIO_yp_arr_s){ \
				.elemsize = sizeof((ARR)[0]), \
				.count = (COUNT), \
				.arr = (ARR), \
				.sep = (SEP), \
				.printfunc = YYΩIO_PRINT_FUNC_GENERIC((ARR)[0]),
			})
#define YYIO_yp_arr_N(_3,_2,_1,N,...)  YYIO_yp_arr_##N
/**
 * @def pr_arr
 * @brief Print an array using custom format specifier and separator.
 * The format string is used for each element.
 * @exmaple
 *     int arr[] = {4, 3, 2, 1};
 *     yprintf("{}", yp_arr(arr));             // -> "4, 3, 2, 1"
 *     yprintf("{}", yp_arr(arr, "|"));        // -> "4|3|2|1"
 *     yprintf("{:02x}", yp_arr(arr, "|"));    // -> "04|03|02|01"
 *     int *pnt = arr;
 *     yprintf("{:02x}", yp_arr(pnt, 3, "|"));  // -> "04|03|02"
 */
#define yp_arr(...)  YYIO_yp_arr_N(__VA_ARGS__, 3, 2, 1)(__VA_ARGS__)

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
 *    yprintf(F("{var=:4d}"));  // -> yprintf("var={:4d}", var);
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

int main() {
	int var = 1;
	int var2 = 2;
	yprintf(F("pre {var} post"));
	yprintf(F("pre {var1} {var2} post"));
	yprintf(F("pre {{ }} {{ {var1} {{ {var2} }} {{ post"));
	yprintf(F("pre {{}} {var1} {{ {var2} }} {{ post"));
	yprintf(F("pre {0} {var} post", 1));
	yprintf(F("pre {{}} {var1:{var2}} {{ post"));
	yprintf(F("pre {{}} {var1=:{var2}} {{ post"));
}
