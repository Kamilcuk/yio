/**
 * @file
 * @date 2020-06-11
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE  1
#define _XOPEN_SOURCE  700
#define _POSIX_C_SOURCE  200809L
#define _BSD_SOURCE  1
#define _SVID_SOURCE  1
#define _DEFAULT_SOURCE  1
#define __STDC_WANT_LIB_EXT2__  1
#define __STDC_WANT_DEC_FP__  1
#define __STDC_WANT_IEC_60559_EXT__  1
#define __STDC_WANT_IEC_60559_DFP_EXT__  1
#define __STDC_WANT_IEC_60559_BFP_EXT__  1
#define __STDC_WANT_IEC_60559_FUNCS_EXT__  1
#define __STDC_WANT_IEC_60559_TYPES_EXT__  1
#include "../private/yio_float.h"
#include <stdio.h>
#include <errno.h>
#ifndef YIO_HAS_FLOAT_H
#error
#endif
#if YIO_HAS_FLOAT_H
#include <float.h>
#endif
#ifndef YIO_HAS_FENV_H
#error
#endif
#if YIO_HAS_FENV_H
#include <fenv.h>
#endif
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------------- */

#ifdef __GNUC__
#define weak  __attribute__((__weak__))
#endif

#ifndef ENOSYS
#define	ENOSYS		38	/* Invalid system call number */
#endif

static inline void floaterror(const char *func) {
	(void)fprintf(stderr,
			"yio: compatlib: Function %s is not implemented. Link with an actual implementation.\n",
			func);
	errno = ENOSYS;
#if YIO_HAS_FENV_H
	feraiseexcept(FE_INVALID);
#endif
}

#define ERROR()  floaterror(__func__)

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}{% call j_APPLY(V.1, V.math, V.strto) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error YIO_HAS_FLOAT$1 not defined
#endif

#if YIO_HAS_FLOAT$1

{% set is_decimal = j_search(V.1, "^d") %}

#ifndef YIO_HAS_floor$1
#error YIO_HAS_floor$1 not defined
#endif
#if !YIO_HAS_floor$1
{% if V.2 %}
weak YIO_FLOAT$1 floor$2(YIO_FLOAT$1 x);
weak YIO_FLOAT$1 floor$2(YIO_FLOAT$1 x) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)floorl((long double)x);
{% else %}
	return (YIO_FLOAT$1)floor((double)x);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 floor$2(YIO_FLOAT$1 x);
weak YIO_FLOAT$1 floor$2(YIO_FLOAT$1 x) {
	ERROR();
	return x;
}
{% endif %}
#endif

#ifndef YIO_HAS_fabs$1
#error YIO_HAS_fabs$1 not defined
#endif
#if !YIO_HAS_fabs$1
weak YIO_FLOAT$1 fabs$2(YIO_FLOAT$1 x);
weak YIO_FLOAT$1 fabs$2(YIO_FLOAT$1 x) {
	return x > 0 ? x : -x;
}
#endif

#ifndef YIO_HAS_exp2$1
#error YIO_HAS_exp2$1 not defined
#endif
#if !YIO_HAS_exp2$1
{% if V.2 %}
weak YIO_FLOAT$1 exp2$2(YIO_FLOAT$1 y);
weak YIO_FLOAT$1 exp2$2(YIO_FLOAT$1 y) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)exp2l((long double)y);
{% else %}
	return (YIO_FLOAT$1)exp2((double)y);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 exp2$2(YIO_FLOAT$1 y);
weak YIO_FLOAT$1 exp2$2(YIO_FLOAT$1 y) {
	ERROR();
	return y;
}
{% endif %}
#endif

#ifndef YIO_HAS_exp10$1
#error YIO_HAS_exp10$1 not defined
#endif
#if !YIO_HAS_exp10$1
weak YIO_FLOAT$1 exp10$2(YIO_FLOAT$1 y);
weak YIO_FLOAT$1 exp10$2(YIO_FLOAT$1 y) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)powl(10.0L, (long double)y);
{% else %}
	return (YIO_FLOAT$1)pow(10.0, (double)y);
{% endif %}
}
#endif



#ifndef YIO_HAS_log2$1
#error YIO_HAS_log2$1 not defined
#endif
#if !YIO_HAS_log2$1
{% if V.2 %}
weak YIO_FLOAT$1 log2$2(YIO_FLOAT$1 x);
weak YIO_FLOAT$1 log2$2(YIO_FLOAT$1 x) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)log2l((long double)x);
{% else %}
	return (YIO_FLOAT$1)log2((double)x);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 log2$2(YIO_FLOAT$1 x);
weak YIO_FLOAT$1 log2$2(YIO_FLOAT$1 x) {
	ERROR();
	return x;
}
{% endif %}
#endif

#ifndef YIO_HAS_log10$1
#error YIO_HAS_log10$1 not defined
#endif
#if !YIO_HAS_log10$1
{% if V.2 %}
weak YIO_FLOAT$1 log10$2(YIO_FLOAT$1 y);
weak YIO_FLOAT$1 log10$2(YIO_FLOAT$1 y) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)log10l((long double)y);
{% else %}
	return (YIO_FLOAT$1)log10((double)y);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 log10$2(YIO_FLOAT$1 y);
weak YIO_FLOAT$1 log10$2(YIO_FLOAT$1 y) {
	ERROR();
	return y;
}
{% endif %}
#endif

#ifndef YIO_HAS_frexp$1
#error YIO_HAS_frexp$1 not defined
#endif
#if !YIO_HAS_frexp$1
{% if V.2 %}
weak YIO_FLOAT$1 frexp$2(YIO_FLOAT$1 val, int *exp);
weak YIO_FLOAT$1 frexp$2(YIO_FLOAT$1 val, int *exp) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)frexpl((long double)val, exp);
{% else %}
	return (YIO_FLOAT$1)frexp((double)val, exp);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 frexp$2(YIO_FLOAT$1 val, int *exp);
weak YIO_FLOAT$1 frexp$2(YIO_FLOAT$1 val, int *exp) {
	ERROR();
	return val;
}
{% endif %}
#endif

#ifndef YIO_HAS_modf$1
#error YIO_HAS_modf$1 not defined
#endif
#if !YIO_HAS_modf$1
{% if V.2 %}
weak YIO_FLOAT$1 modf$2(YIO_FLOAT$1 x, YIO_FLOAT$1 *iptr);
weak YIO_FLOAT$1 modf$2(YIO_FLOAT$1 x, YIO_FLOAT$1 *iptr) {
{% if not is_decimal %}
    long double i;
    long double f = modfl((long double)x, &i);
    *iptr = (YIO_FLOAT$1)i;
    return (YIO_FLOAT$1)f;
{% else %}
    double i;
    double f = modf((double)x, &i);
    *iptr = (YIO_FLOAT$1)i;
    return (YIO_FLOAT$1)f;
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 modf$2(YIO_FLOAT$1 x, YIO_FLOAT$1 *iptr);
weak YIO_FLOAT$1 modf$2(YIO_FLOAT$1 x, YIO_FLOAT$1 *iptr) {
	ERROR();
	return x;
}
{% endif %}
#endif

#ifndef YIO_HAS_pow$1
#error YIO_HAS_pow$1 not defined
#endif
#if !YIO_HAS_pow$1
{% if V.2 %}
weak YIO_FLOAT$1 pow$2(YIO_FLOAT$1 val, YIO_FLOAT$1 to);
weak YIO_FLOAT$1 pow$2(YIO_FLOAT$1 val, YIO_FLOAT$1 to) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)powl((long double)val, (long double)to);
{% else %}
	return (YIO_FLOAT$1)pow((double)val, (double)to);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 pow$2(YIO_FLOAT$1 val, YIO_FLOAT$1 to);
weak YIO_FLOAT$1 pow$2(YIO_FLOAT$1 val, YIO_FLOAT$1 to) {
	ERROR();
	return val;
}
{% endif %}
#endif

#ifndef YIO_HAS_nextafter$1
#error YIO_HAS_nextafter$1 not defined
#endif
#if !YIO_HAS_nextafter$1
{% if V.2 %}
weak YIO_FLOAT$1 nextafter$2(YIO_FLOAT$1 x, YIO_FLOAT$1 y);
weak YIO_FLOAT$1 nextafter$2(YIO_FLOAT$1 x, YIO_FLOAT$1 y) {
{% if not is_decimal %}
	return (YIO_FLOAT$1)nextafterl((long double)x, (long double)y);
{% else %}
	return (YIO_FLOAT$1)nextafter((double)x, (double)y);
{% endif %}
}
{% else %}
weak YIO_FLOAT$1 nextafter$2(YIO_FLOAT$1 x, YIO_FLOAT$1 y);
weak YIO_FLOAT$1 nextafter$2(YIO_FLOAT$1 x, YIO_FLOAT$1 y) {
	ERROR();
	return x;
}
{% endif %}
#endif

#ifndef YIO_HAS_strfrom$1
#error YIO_HAS_strfrom$1 not defined
#endif
#if !YIO_HAS_strfrom$1
weak int strfrom$2(char *restrict str, size_t n, const char *restrict format, YIO_FLOAT$1 fp);
weak int strfrom$2(char *restrict str, size_t n, const char *restrict format, YIO_FLOAT$1 fp) {
	errno = ENOSYS;
	return -1;
}
#endif

#ifndef YIO_HAS_strto$1
#error YIO_HAS_strto$1 not defined
#endif
#if !YIO_HAS_strto$1
weak YIO_FLOAT$1 strto$3(const char *restrict str, char **restrict pnt);
weak YIO_FLOAT$1 strto$3(const char *restrict str, char **restrict pnt) {
	errno = ENOSYS;
	return -1;
}
#endif


#endif // YIO_HAS_FLOAT$1

{% endcall %}{% endfor %}
