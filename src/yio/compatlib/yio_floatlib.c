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
#ifndef YYIO_HAS_FLOAT_H
#error
#endif
#if YYIO_HAS_FLOAT_H
#include <float.h>
#endif
#ifndef YYIO_HAS_FENV_H
#error
#endif
#if YYIO_HAS_FENV_H
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
	fprintf(stderr,
			"yio: compatlib: Function %s is not implemented. Link with an actual implementation.\n",
			func);
	errno = ENOSYS;
#if YYIO_HAS_FENV_H
	feraiseexcept(FE_INVALID);
#endif
}

#define ERROR()  floaterror(__func__)

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}{% call j_APPLY(V.0, V.math, V.strto) %}
#line

#ifndef YYIO_HAS_FLOAT$1
#error  YYIO_HAS_FLOAT$1
#endif
#if YYIO_HAS_FLOAT$1

weak YYIO_FLOAT$1 floor$2(YYIO_FLOAT$1 x);
weak YYIO_FLOAT$1 floor$2(YYIO_FLOAT$1 x) {
	return (YYIO_FLOAT$1)(int)x;
}

weak YYIO_FLOAT$1 fabs$2(YYIO_FLOAT$1 x);
weak YYIO_FLOAT$1 fabs$2(YYIO_FLOAT$1 x) {
	return x > 0 ? x : -x;
}

#ifdef exp2$2
#undef exp2$2
#endif
weak YYIO_FLOAT$1 exp2$2(YYIO_FLOAT$1 y);
weak YYIO_FLOAT$1 exp2$2(YYIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef round$2
#undef round$2
#endif
weak YYIO_FLOAT$1 exp10$2(YYIO_FLOAT$1 y);
weak YYIO_FLOAT$1 exp10$2(YYIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef round$2
#undef round$2
#endif
weak YYIO_FLOAT$1 round$2(YYIO_FLOAT$1 y);
weak YYIO_FLOAT$1 round$2(YYIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef log2$2
#undef log2$2
#endif
weak YYIO_FLOAT$1 log2$2(YYIO_FLOAT$1 x);
weak YYIO_FLOAT$1 log2$2(YYIO_FLOAT$1 x) {
	// https://stackoverflow.com/questions/39864840/using-series-to-approximate-log2
	unsigned long n = 1;
	const YYIO_FLOAT$1 eps = YYIO_FLOAT_C$1(0.00001);
	YYIO_FLOAT$1 kpow = (x - 1) / (x + 1);
	YYIO_FLOAT$1 kpow2 = kpow * kpow;
	YYIO_FLOAT$1 dk;
	YYIO_FLOAT$1 k = 2 * kpow;

	do {
	    n += 2;
	    kpow *= kpow2;
	    dk = YYIO_FLOAT_C$1(2.0) * kpow / (YYIO_FLOAT$1)n;
	    k += dk;
	} while (fabs$2(dk) >= eps);

	return k;
}

#ifdef log10$2
#undef log10$2
#endif
weak YYIO_FLOAT$1 log10$2(YYIO_FLOAT$1 y);
weak YYIO_FLOAT$1 log10$2(YYIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef frexp$2
#undef frexp$2
#endif
weak YYIO_FLOAT$1 frexp$2(YYIO_FLOAT$1 val, int *exp);
weak YYIO_FLOAT$1 frexp$2(YYIO_FLOAT$1 val, int *exp) {
	ERROR();
	return NAN;
}

#ifdef frexp$2
#undef frexp$2
#endif
weak YYIO_FLOAT$1 pow$2(YYIO_FLOAT$1 val, YYIO_FLOAT$1 to);
weak YYIO_FLOAT$1 pow$2(YYIO_FLOAT$1 val, YYIO_FLOAT$1 to) {
	ERROR();
	return NAN;
}

int strfrom$1(char *restrict str, size_t n, const char *restrict format, YYIO_FLOAT$1 fp);
weak int strfrom$1(char *restrict str, size_t n, const char *restrict format, YYIO_FLOAT$1 fp) {
	errno = ENOSYS;
	return -1;
}

YYIO_FLOAT$1 strto$3(const char *restrict str, char **restrict pnt);
weak YYIO_FLOAT$1 strto$3(const char *restrict str, char **restrict pnt) {
	errno = ENOSYS;
	return -1;
}

#endif // YYIO_HAS_FLOAT$1

{% endcall %}{% endfor %}


