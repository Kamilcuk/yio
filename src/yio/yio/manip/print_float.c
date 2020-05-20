/**
 * @file print_float.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config();
#define __STDC_WANT_DEC_FP__  1
#define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#define __STDC_WANT_IEC_60559_FUNCS_EXT__ 1
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#include "private.h"
#include <float.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <math.h>

static inline
const char *_yIO_printfmt_to_cfmt_for_floats_in(
		char *dest, size_t destsize,
		int precision, char type) {
	const int len = ysprint(dest, destsize, "%.", precision, type);
	(void)len;
	assert(len < (int)destsize);
	return dest;
}

#define _yIO_printfmt_to_cfmt_for_floats_buflen \
		(INT_STRLEN_BOUND(int) + sizeof("%.f"))

#define _yIO_printfmt_to_cfmt_for_floats(precision, type) \
		_yIO_printfmt_to_cfmt_for_floats_in( \
				(char[_yIO_printfmt_to_cfmt_for_floats_buflen]){0}, \
				_yIO_printfmt_to_cfmt_for_floats_buflen, \
				precision, type)

/**
 * m4__yIO_print_generic(type, name, suffix, strfromx, strtox)
 */
m4_define(`m4__yIO_print_generic~,`m4_dnl;
m4_pushdef(`TYPE~, `$1~)m4_dnl;
m4_dnl;
m4_pushdef(`NAME~, ``_yIO_print_~$2~)m4_dnl;
m4_pushdef(`NAME_in~, ``_yIO_print_~$2`_in~~)m4_dnl;
m4_dnl;
m4_pushdef(`TYPE_DECIMAL_DIG~, `$3_DECIMAL_DIG~)m4_dnl;
m4_pushdef(`__TYPE_DECIMAL_DIG__~, `__$3_DECIMAL_DIG__~)m4_dnl;
m4_pushdef(`_yIO_HAVE_TYPE~, ``_yIO_HAVE_~$3~)m4_dnl;
m4_dnl;
m4_pushdef(`STRFROMX~, `$4~)m4_dnl;
m4_pushdef(`_yIO_HAVE_STRFROMX~, ``_yIO_HAVE_~$4~)m4_dnl;

// on newlib, TYPE_DECIMAL_DIG is missing
// but gcc defines __TYPE_DECIMAL_DIG__
// so we can handle it
#ifndef TYPE_DECIMAL_DIG
#ifdef __TYPE_DECIMAL_DIG__
#define TYPE_DECIMAL_DIG  __TYPE_DECIMAL_DIG__
#endif
#endif

// let's just assume 10 is the default for now
#ifndef TYPE_DECIMAL_DIG
#define TYPE_DECIMAL_DIG  10
#endif

#ifdef _yIO_HAVE_TYPE

static inline
int NAME_in()(yio_printctx_t *t, TYPE val) {
	const struct yio_printfmt_s *pf = &t->pf;
	const char type = pf->type ? pf->type : 'g';
	if (strchr("aAeEfFgG", type) == NULL) {
		return EINVAL;
	}
	const int precision = pf->precision < 0 ? TYPE_DECIMAL_DIG : pf->precision;

#ifdef _yIO_HAVE_STRFROMX
	int ret = 0;
	const char *cformat = _yIO_printfmt_to_cfmt_for_floats(precision, type);
	const int len = $4(NULL, 0, cformat, val);
	assert(len >= 0);
	char * const str = malloc((len + 1) * sizeof(str));
	if (str == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto EXIT;
	}
	const int len2 = STRFROMX()(str, len + 1, cformat, val);
	(void)len2;
	assert(len2 == len);
	const bool is_negative = str[0] == '-';
	ret = _yIO_printformat_number(t, str + is_negative, !is_negative);
	free(str);

	EXIT:
	return ret;
#else // _yIO_HAVE_STRFROMX

	(void)type;
	(void)precision;
	return ENOSYS;

#endif // _yIO_HAVE_STRFROMX
}

int NAME()(yio_printctx_t *t) {
	const TYPE f = yio_printctx_va_arg_num(t, TYPE);
	return NAME_in()(t, f);
}

#endif // _yIO_HAVE_TYPE

m4_popdef(`NAME~)m4_dnl;
m4_popdef(`TYPE~, `$2~)m4_dnl;
m4_popdef(`TYPE_DECIMAL_DIG~, `$3_DECIMAL_DIG~)m4_dnl;
m4_popdef(`STRFROMX~, `$4~)m4_dnl;
m4_popdef(`PROMOTEDTYPE~, `m4_ifelse(`$2~, `float~, `double~, `$2~)~)m4_dnl;
m4_popdef(`_yIO_HAVE_TYPE~, `_yIO_HAVE`~$3~)m4_dnl;
m4_popdef(`_yIO_HAVE_STRFROMX~, `_yIO_HAVE`~STRFROMX~)m4_dnl;
~) m4_dnl;

m4_include(`floatlist.m4~)
m4_applyforeach(`m4__yIO_print_generic~, m4_floatlist)

#ifdef _yIO_HAVE_float
int _yIO_print_floatptr(yio_printctx_t *t) {
	const float *fptr = yio_printctx_va_arg(t, float*);
	assert(fptr != NULL);
	const float f = *fptr;
	return _yIO_print_float_in(t, f);
}
#endif

