/**
 * @file print_float.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config();
#define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#define __STDC_WANT_DEC_FP__  1
#include "private.h"
#include <float.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

static inline
const char *_yIO_printfmt_to_cfmt_for_floats_in(
		char *dest, size_t destsize,
		struct yio_printfmt_s *f,
		int default_precision) {
	const char type = f->type ? f->type : 'g';
	assert(strchr("aAeEfFgG", type) != NULL);
	const int precision = f->precision < 0 ? default_precision : f->precision;
	const int len = snprintf(dest, destsize, "%%.%d%c", precision, type);
	(void)len;
	assert(len < (int)destsize);
	return dest;
}

#define _yIO_printfmt_to_cfmt_for_floats_buflen \
		(INT_STRLEN_BOUND(int) + sizeof("%.f"))

#define _yIO_printfmt_to_cfmt_for_floats(fmt, default_width) \
		_yIO_printfmt_to_cfmt_for_floats_in( \
				(char[_yIO_printfmt_to_cfmt_for_floats_buflen]){0}, \
				_yIO_printfmt_to_cfmt_for_floats_buflen, \
				fmt, \
				default_width)

m4_divert(-1)
#define m4__yIO_print_generic(FUNCTION_NAME, type, MAX_RESOLUTION, strfrom, ...)
m4_define(`m4__yIO_print_generic~,`
#ifdef $3
static inline
int $1_in(yio_printctx_t *t, $2 f) {
	int ret = 0;
	const char *cformat = _yIO_printfmt_to_cfmt_for_floats(yio_printctx_get_fmt(t), $3);
	const int len = $4(NULL, 0, cformat, f);
	assert(len >= 0);
	char * const str = malloc((len + 1) * sizeof(str));
	if (str == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto EXIT;
	}
	const int len2 = $4(str, len + 1, cformat, f);
	(void)len2;
	assert(len2 == len);
	const bool is_negative = str[0] == '-';
	ret = _yIO_printformat_number(t, str + is_negative, !is_negative);
	free(str);

	EXIT:
	return ret;
}
m4_ifelse($5,,
int $1(yio_printctx_t *t) {
	const $2 f = yio_printctx_va_arg_num(t, $2);
	return _yIO_print_double_in(t, f);
}
)m4_dnl
#endif
~)
m4_divert(0)m4_dnl;

m4__yIO_print_generic(_yIO_print_float, float, FLT_DECIMAL_DIG, strfromf, NO)
#ifdef DBL_DECIMAL_DIG
m4__yIO_print_generic(_yIO_print_double_that_is_float, double, FLT_DECIMAL_DIG, strfromf, NO)
#endif
m4__yIO_print_generic(_yIO_print_double, double, DBL_DECIMAL_DIG, strfromd)
m4__yIO_print_generic(_yIO_print_ldouble, long double, LDBL_DECIMAL_DIG, strfroml)
#if _yIO_USE__FLOATN
m4__yIO_print_generic(_yIO_print__Float16, _Float16, FLT16_DECIMAL_DIG, strfromf16)
m4__yIO_print_generic(_yIO_print__Float32, _Float32, FLT32_DECIMAL_DIG, strfromf32)
m4__yIO_print_generic(_yIO_print__Float64, _Float64, FLT64_DECIMAL_DIG, strfromf64)
m4__yIO_print_generic(_yIO_print__Float128, _Float128, FLT128_DECIMAL_DIG, strfromf128)
m4__yIO_print_generic(_yIO_print__Float32x, _Float32x, FLT32X_DECIMAL_DIG, strfromf32x)
m4__yIO_print_generic(_yIO_print__Float64x, _Float64x, FLT64X_DECIMAL_DIG, strfromf64x)
m4__yIO_print_generic(_yIO_print__Float128x, _Float128x, FLT128X_DECIMAL_DIG, strfromf128x)
#endif
#if _yIO_USE__DECIMALN
m4__yIO_print_generic(_yIO_print__Decimal32, _Decimal32, DEC32_DECIMAL_DIG, strfromd32)
m4__yIO_print_generic(_yIO_print__Decimal64, _Decimal64, DEC64_DECIMAL_DIG, strfromd64)
m4__yIO_print_generic(_yIO_print__Decimal128, _Decimal128, DEC128_DECIMAL_DIG, strfromd128)
m4__yIO_print_generic(_yIO_print__Decimal32x, _Decimal32x, DEC32_DECIMAL_DIG, strfromd32x)
m4__yIO_print_generic(_yIO_print__Decimal64x, _Decimal64x, DEC64_DECIMAL_DIG, strfromd64x)
m4__yIO_print_generic(_yIO_print__Decimal128x, _Decimal128x, DEC128_DECIMAL_DIG, strfromd128x)
#endif

int _yIO_print_floatptr(yio_printctx_t *t) {
	const float *fptr = yio_printctx_va_arg(t, float*);
	assert(fptr != NULL);
	const float f = *fptr;
	return _yIO_print_float_in(t, f);
}

int _yIO_print_float(yio_printctx_t *t) {
	const double f = yio_printctx_va_arg_num(t, float);
	return _yIO_print_double_that_is_float_in(t, f);
}
