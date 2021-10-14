/**
 * @file
 * @date 2020-06-11
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE  1
#define __STDC_WANT_DEC_FP__  1
#define __STDC_WANT_IEC_60559_EXT__  1
#define __STDC_WANT_IEC_60559_DFP_EXT__  1
#include "yio_decimallib.h"
#include <stdio.h>

#ifdef __GNUC__
#define weak __attribute__((__weak__))
#endif

#define ERROR()  do { \
	fprintf(stderr, "yio: decimallib: Function %s is not implemented. Link with an actual implementation.", __func__); \
	abort(); \
} while (0)

m4_applyforeachdefine(«(
			(d32, 32, df),
			(d64, 64, dd),
			(d128,128, dl),
)», m4_syncline()«m4_dnl;

#if _yIO_HAS_FLOAT$1

#if __STDC_IEC_60559_DFP__ > 0

#define _yIO_FLOAT_C$1(x)  x ## $3

weak _Decimal$2 floor$1(_Decimal$2 x) {
	return (int)x;
}

weak _Decimal$2 fabs$1(_Decimal$2 x) {
	return x > 0 ? x : -x;
}

weak _Decimal$2 exp2$1(_Decimal$2 y) {
	ERROR();
}

weak _Decimal$2 exp10$1(_Decimal$2 y) {
	ERROR();
}

weak _Decimal$2 round$1(_Decimal$2 y) {
	ERROR();
}

weak _Decimal$2 log2$1(_Decimal$2 x) {
	// https://stackoverflow.com/questions/39864840/using-series-to-approximate-log2
	unsigned long n = 1;
	_Decimal$2 eps = _yIO_FLOAT_C$1(0.00001);
	_Decimal$2 kpow = (x - 1) / (x + 1);
	_Decimal$2 kpow2 = kpow * kpow;
	_Decimal$2 dk;
	_Decimal$2 k = 2 * kpow;

	do {
	    n += _yIO_FLOAT_C$1(2.0);
	    kpow *= kpow2;
	    dk = _yIO_FLOAT_C$1(2.0) * kpow / n;
	    k += dk;
	} while (fabs$1(dk) >= eps);

	return dk;
}

weak _Decimal$2 log10$1(_Decimal$2 y) {
	ERROR();
}

weak _Decimal$2 frexp$1(_Decimal$2 val, int *exp) {
	ERROR();
}

weak int strfrom$1(char *restrict str, size_t n, const char *restrict format, _Decimal$2 fp) {
	ERROR();
}

#endif // _yIO_HAS_IEC_60559_DFP

#endif // _yIO_HAS_DEC$1

») // m4_dnl m4_applyforeachdefine ;

