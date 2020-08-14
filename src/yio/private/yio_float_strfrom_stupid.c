/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE
#include "yio_float_strfrom_stupid.h"
#include "yio_res.h"
#include "yio_float.h"
#include "private.h"

#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>

#define ASSERTMSG(expr, fmt, ...) do { \
	if (!(expr)) { \
		fprintf(stderr, "%s:%d: ERROR: %s failed: " fmt "\n", \
				__func__, __LINE__, #expr, ##__VA_ARGS__); \
	} \
} while (0)

static const char _yIO_NAN[3] = {'N','A','N'};
static const char _yIO_nan[3] = {'n','a','n'};
static const char _yIO_INF[3] = {'I','N','F'};
static const char _yIO_inf[3] = {'i','n','f'};
static const char _yIO_HEX[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static const char _yIO_hex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

m4_applyforeachdefine(`((f), (d), (l))~, m4_syncline(1)`m4_dnl;

#ifndef _yIO_HAS_FLOAT$1
#error _yIO_HAS_FLOAT$1
#endif

#if _yIO_HAS_FLOAT$1

#define TYPE     _yIO_FLOAT$1
#define PRI      _yIO_FLOAT_PRI$1
#define FLOOR    _yIO_floor$1
#define EXP2     _yIO_exp2$1
#define EXP10    _yIO_exp10$1
#define FABS     _yIO_fabs$1
#define FREXP2   _yIO_frexp2$1
#define FREXP10  _yIO_frexp10$1

static inline
int get_next_digit$1(_yIO_res *v, TYPE *val,
		bool hex, const char *to_digit_str, bool is_last) {
	const TYPE base = hex ? (TYPE)16 : (TYPE)10;
	const int baseint = hex ? 16 : 10;
	*val *= base;

	const int digit = floor(*val);
	ASSERTMSG(digit < baseint,
			"digit=%d hex=%d is_last=%d val=%"PRI"f\n",
			digit, hex, is_last, *val
	);
	const char c = to_digit_str[digit];
	const int err = _yIO_res_putc(v, c);
	if (err) return err;
	if (!is_last) {
		*val -= digit;
	}
	return 0;
}

int _yIO_float_astrfrom_stupid$1(char **resultp, size_t *lengthp, int precision, char spec0, TYPE val) {
	int err = 0;

	char spec = spec0;
	_yIO_res _r_mem;
	_yIO_res * const v = &_r_mem;
	_yIO_res_init(v, resultp, lengthp);

	// take minus out of the way
	const bool negative = signbit(val);
	if (negative) {
		err = _yIO_res_putc(v, '-');
		if (err) return err;
		val = FABS(val);
	}

	// take INF and NAN out of the way
	const bool upper = isupper((unsigned char)spec);
	const char (*nan_or_inf_str)[3] =
			isnan(val) ? upper ? &_yIO_NAN : &_yIO_nan :
					isinf(val) ? upper ? &_yIO_INF : &_yIO_inf : NULL;
	if (nan_or_inf_str) {
		err = _yIO_res_putsn(v, *nan_or_inf_str, 3);
		if (err) return err;
		goto SUCCESS;
	}

	// all the happy rest

	// fF 987.654
	// eE 9.87654e+02
	// aA 1.<hex>p+1
	// gG if <exponent> then f else e
	const char spec0lower = tolower(spec);
	char speclower = spec0lower;
	bool hex = speclower == 'a';
	bool scientific = speclower == 'a' || speclower == 'e';
	const bool is_lower_spec = speclower == spec;
	const char * const to_digit_str = is_lower_spec ? _yIO_hex : _yIO_HEX;

	if (precision < 0) {
		precision = 6;
	}

	bool val_is_zero = val == 0;

	if (speclower == 'g') {
		if (precision == 0) {
			precision = 1;
		}

		int exponenttmp = 0;
		FREXP10(val, &exponenttmp);
		const TYPE val1 = val + (TYPE)0.05 * EXP10(-precision + exponenttmp);
		FREXP10(val1, &exponenttmp);

		if (precision > exponenttmp && exponenttmp >= -4) {
			spec = is_lower_spec ? 'f' : 'F';
			speclower = 'f';
			scientific = false;
			precision += -exponenttmp;
		} else {
			spec = is_lower_spec ? 'e' : 'E';
			speclower = 'e';
			scientific = true;
			precision -= 1;
		}
	}

	int exponent;
	if (val_is_zero) {
		exponent = 0;
	} else if (speclower == 'f') {
		val += (TYPE)0.5 / EXP10(precision);
		val = FREXP10(val, &exponent);
	} else if (speclower == 'e') {
		FREXP10(val, &exponent);
		val += (TYPE)0.05 * EXP10(-precision + exponent);
		val = FREXP10(val, &exponent);
	} else {
		val = FREXP2(val, &exponent);
		// decrease exponent
		val *= 2;
		// rounding
		val += EXP2(-1 + -4 * precision);
	}

	if (speclower == 'f') {
		if (exponent <= 0) {
			err = _yIO_res_putc(v, '0');
			if (err) return err;
		} else {
			assert(exponent > 0);
			for (int i = exponent; i; --i) {
				err = get_next_digit$1(v, &val, hex, to_digit_str, i == 1 && precision == 0);
				if (err) return err;
			}
		}
	} else if (speclower == 'e') {
		if (val_is_zero) {
			err = _yIO_res_putc(v, '0');
			if (err) return err;
		} else {
			err = get_next_digit$1(v, &val, hex, to_digit_str, precision == 0);
			if (err) return err;
		}
	} else if (speclower == 'a') {
		err = _yIO_res_putc(v, '0');
		if (err) return err;
		err = _yIO_res_putc(v, is_lower_spec ? 'x': 'X');
		if (err) return err;
		if (val_is_zero) {
			err = _yIO_res_putc(v, '0');
			if (err) return err;
		} else {
			// print first number
			if (val < 2) {
				err = _yIO_res_putc(v, '1');
				if (err) return err;
				val -= 1;
			} else {
				// it rounded to whole number up
				err = _yIO_res_putc(v, '2');
				if (err) return err;
				// print only zeros below
				val = 0;
			}
		}
	}

	bool g_fractional_part_removed = false;
	if (precision) {
		err = _yIO_res_putc(v, '.');
		if (err) return err;
		int zeros = (speclower == 'f' && exponent < 0) ? -exponent : 0;
		while (precision--) {
			if (zeros) {
				--zeros;
				err = _yIO_res_putc(v, '0');
				if (err) return err;
			} else {
				err = get_next_digit$1(v, &val, hex, to_digit_str, precision == 0);
				if (err) return err;
			}
		}
		if (spec0lower == 'g') {
			g_fractional_part_removed = _yIO_res_remove_trailing_zeros(v);
		}
	}

	const bool print_scientif_suffix = scientific &&
			!(spec0lower == 'g' && !val_is_zero && exponent == 1 && !g_fractional_part_removed);
	if (print_scientif_suffix) {
		const char letter = !hex ? spec :
				is_lower_spec ? 'p' : 'P';
		err = _yIO_res_putc(v, letter);
		if (err) return err;
		err = _yIO_res_yprintf(v, "{:+0{}}", val_is_zero ? 0 : (exponent - 1), hex ? 0 : 3);
		if (err) return err;
	}

	SUCCESS:
	_yIO_res_end(v, resultp, lengthp);
	return 0;
}

#undef TYPE
#undef PRI
#undef FLOOR
#undef EXP2
#undef EXP10
#undef FABS
#undef FREXP2
#undef FREXP10

#endif // _yIO_HAS_FLOAT$1
~)
