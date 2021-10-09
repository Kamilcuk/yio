/**
 * @file
 * @date 2020-06-11
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define __STDC_WANT_IEC_60559_DFP_EXT__
#include "yio_decimallib.h"
#include <stdbool.h>
#include <math.h>

#if __STDC_IEC_60559_DFP__ > 0
#define _yIO_HAS_IEC_60559_DFP
#endif

m4_applyforeachdefine(
«(
		( 32, df),
		( 64, dd),
		(128, dl),
)»,«m4_dnl;

#define _yIO_FLOAT_Cd$1(x)  x ## $2

/* suffix $1 ---------------------------------------------------- */

#if _yIO_HAS_FLOATd$1
#if _yIO_HAS_IEC_60559_DFP

_Decimal$1 frexp10d$1(_Decimal$1 val, int *exp) {
	return frexpd$1(val, exp);
}

#else // _yIO_HAS_IEC_60559_DFP

_Decimal$1 floord$1(_Decimal$1 x) {
	return (int)x;
}

_Decimal$1 fabsd$1(_Decimal$1 x) {
	return x > 0 ? x : -x;
}

_Decimal$1 _yIO_expintXd$1(_Decimal$1 y, _Decimal$1 X) {
	if (y == 0) {
		return 1;
	}

	const bool negative = y < 0;
	if (negative) {
		y = fabsd$1(y);
	}

	_Decimal$1 res = X;
	while (--y > 0) {
		res *= X;
	}

	return negative ? 1 / res : res;
}

_Decimal$1 exp2d$1(_Decimal$1 y) {
	return _yIO_expintXd$1(y, 2);
}


_Decimal$1 exp10d$1(_Decimal$1 y) {
	return _yIO_expintXd$1(y, 10);
}

_Decimal$1 roundd$1(_Decimal$1 y) {
	y += _yIO_FLOAT_Cd$1(0.5);
	return y - (y - (int)y);
}

_Decimal$1 log2d$1(_Decimal$1 x) {
	// https://stackoverflow.com/questions/39864840/using-series-to-approximate-log2
	unsigned long n = 1;
	_Decimal$1 eps = _yIO_FLOAT_Cd$1(0.00001);
	_Decimal$1 kpow = (x - 1) / (x + 1);
	_Decimal$1 kpow2 = kpow * kpow;
	_Decimal$1 dk;
	_Decimal$1 k = 2 * kpow;

	do {
	    n += _yIO_FLOAT_Cd$1(2.0);
	    kpow *= kpow2;
	    dk = _yIO_FLOAT_Cd$1(2.0) * kpow / n;
	    k += dk;
	} while (fabsd$1(dk) >= eps);

	return dk;
}

_Decimal$1 log10d$1(_Decimal$1 y) {
	static const _Decimal$1 log2_of_10_d$1 = _yIO_FLOAT_Cd$1(
			2.30258509299404568401799145468436420760110148862877297603332790096
	);
	return log2d$1(y) / log2_of_10_d$1;
}

_Decimal$1 frexpd$1(_Decimal$1 val, int *exp) {
	// TODO: User proper bitmasks...
	if (val == 0) {
		*exp = 0;
	} else {
		const int tmp = 1 + floord$1(log10d$1(val));
		*exp = tmp;
		val *= exp10d$1(-tmp);
		if (val < _yIO_FLOAT_Cd$1(0.1)) {
			val = _yIO_FLOAT_Cd$1(0.1);
		} else if (val > _yIO_FLOAT_Cd$1(1.0)) {
			val = _yIO_FLOAT_Cd$1(1.0);
		}
	}
	return val;
}

#endif // _yIO_HAS_IEC_60559_DFP

#endif // _yIO_HAS_DEC$1

»)

