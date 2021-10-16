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
#ifndef _yIO_HAS_FLOAT_H
#error
#endif
#if _yIO_HAS_FLOAT_H
#include <float.h>
#include <fenv.h>
#endif
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------------- */

#ifdef __GNUC__
#define weak  __attribute__((__weak__))
#endif

static inline void floaterror(const char *func) {
	fprintf(stderr,
			"yio: compatlib: Function %s is not implemented. Link with an actual implementation.\n",
			func);
	errno = ENOSYS;
#if _yIO_HAS_FLOAT_H
	feraiseexcept(FE_INVALID);
#endif
}

#define ERROR()  floaterror(__func__)

/* ------------------------------------------------------------------------- */

m4_define(«m4_worktodo», m4_syncline()«

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

weak _yIO_FLOAT$1 floor$2(_yIO_FLOAT$1 x);
weak _yIO_FLOAT$1 floor$2(_yIO_FLOAT$1 x) {
	return (int)x;
}

weak _yIO_FLOAT$1 fabs$2(_yIO_FLOAT$1 x);
weak _yIO_FLOAT$1 fabs$2(_yIO_FLOAT$1 x) {
	return x > 0 ? x : -x;
}

#ifdef exp2$2
#undef exp2$2
#endif
weak _yIO_FLOAT$1 exp2$2(_yIO_FLOAT$1 y);
weak _yIO_FLOAT$1 exp2$2(_yIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef round$2
#undef round$2
#endif
weak _yIO_FLOAT$1 exp10$2(_yIO_FLOAT$1 y);
weak _yIO_FLOAT$1 exp10$2(_yIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef round$2
#undef round$2
#endif
weak _yIO_FLOAT$1 round$2(_yIO_FLOAT$1 y);
weak _yIO_FLOAT$1 round$2(_yIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef log2$2
#undef log2$2
#endif
weak _yIO_FLOAT$1 log2$2(_yIO_FLOAT$1 x);
weak _yIO_FLOAT$1 log2$2(_yIO_FLOAT$1 x) {
	// https://stackoverflow.com/questions/39864840/using-series-to-approximate-log2
	unsigned long n = 1;
	_yIO_FLOAT$1 eps = _yIO_FLOAT_C$1(0.00001);
	_yIO_FLOAT$1 kpow = (x - 1) / (x + 1);
	_yIO_FLOAT$1 kpow2 = kpow * kpow;
	_yIO_FLOAT$1 dk;
	_yIO_FLOAT$1 k = 2 * kpow;

	do {
	    n += _yIO_FLOAT_C$1(2.0);
	    kpow *= kpow2;
	    dk = _yIO_FLOAT_C$1(2.0) * kpow / n;
	    k += dk;
	} while (fabs$2(dk) >= eps);

	return dk;
}

#ifdef log10$2
#undef log10$2
#endif
weak _yIO_FLOAT$1 log10$2(_yIO_FLOAT$1 y);
weak _yIO_FLOAT$1 log10$2(_yIO_FLOAT$1 y) {
	ERROR();
	return NAN;
}

#ifdef frexp$2
#undef frexp$2
#endif
weak _yIO_FLOAT$1 frexp$2(_yIO_FLOAT$1 val, int *exp);
weak _yIO_FLOAT$1 frexp$2(_yIO_FLOAT$1 val, int *exp) {
	ERROR();
	return NAN;
}

#ifdef frexp$2
#undef frexp$2
#endif
weak _yIO_FLOAT$1 pow$2(_yIO_FLOAT$1 val, _yIO_FLOAT$1 to);
weak _yIO_FLOAT$1 pow$2(_yIO_FLOAT$1 val, _yIO_FLOAT$1 to) {
	ERROR();
	return NAN;
}

#endif // _yIO_HAS_FLOAT$1

»)

m4_applyforeachdefine(«(
			(f), (d), (l),
			(f16), (f32), (f64), (f128),
			(f32x), (f64x), (f128x),
			(d32), (d64), (d128),
			(d32x), (d64x), (d128x),
)», m4_syncline()«m4_dnl;

m4_worktodo($1, m4_ifelse(«$1», «d», «», «$1»))

») // m4_dnl m4_applyforeachdefine ;

/* ------------------------------------------------------------------------- */

m4_applysync(«(
			(f), (s), (l),
			(f16), (f32), (f64), (f128),
			(f32x), (f64x), (f128x),
			(d32), (d64), (d128),
			(d32x), (d64x), (d128x),
)», «

#if _yIO_HAS_FLOAT$1
int strfrom$1(char *restrict str, size_t n, const char *restrict format, _yIO_FLOAT$1 fp);
weak int strfrom$1(char *restrict str, size_t n, const char *restrict format, _yIO_FLOAT$1 fp) {
	errno = ENOSYS;
	return -1;
}
#endif

»)

/* ------------------------------------------------------------------------- */

m4_applysync(«(
			(f), (d), (ld),
			(f16), (f32), (f64), (f128),
			(f32x), (f64x), (f128x),
			(d32), (d64), (d128),
			(d32x), (d64x), (d128x),
)», «

#if _yIO_HAS_FLOAT$1
_yIO_FLOAT$1 strto$1(const char *restrict str, char **restrict pnt);
weak _yIO_FLOAT$1 strto$1(const char *restrict str, char **restrict pnt) {
	errno = ENOSYS;
	return -1;
}
#endif

»)
