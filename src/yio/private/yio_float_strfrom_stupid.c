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
#include "yio_buf.h"
#include "yio_vec.h"
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


m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;
#ifdef _yIO_HAS_FLOAT$1

#define TYPE     _yIO_FLOAT$1
#define PRI      _yIO_FLOAT_PRI$1
#define FLOOR    floor$1
#define EXP2     exp2$1
// newlib doesn't have exp10l, but has expl
#ifdef _yIO_HAS_exp10$1
#define EXP10    exp10$1
#else
#define EXP10(x) pow$1(10.0, x)
#endif
#define FABS     fabs$1
#define FREXP2   _yIO_frexp2$1
#define FREXP10  _yIO_frexp10$1

static inline
int get_next_digit$1(_yIO_vec *v, TYPE *val,
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
	const int err = _yIO_vec_putc(v, c);
	if (err) return err;
	if (!is_last) {
		*val -= digit;
	}
	return 0;
}

int _yIO_float_astrfrom_stupid$1(char **out, int precision, char spec0, TYPE val) {
	int err = 0;
	*out = NULL;

	char spec = spec0;
	_yIO_vec _v_mem;
	_yIO_vec * const v = &_v_mem;
	_yIO_vec_init(v);

	// take minus out of the way
	const bool negative = signbit(val);
	if (negative) {
		err = _yIO_vec_putc(v, '-');
		if (err) return err;
		val = FABS(val);
	}

	// take INF and NAN out of the way
	const bool upper = isupper((unsigned char)spec);
	const char *nan_or_inf_str =
			isnan(val) ? upper ? "NAN" : "nan" :
					isinf(val) ? upper ? "INF" : "inf" : NULL;
	if (nan_or_inf_str) {
		err = _yIO_vec_puts(v, nan_or_inf_str);
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
	const char *to_digit_str = is_lower_spec ?
					"0123456789abcdef" :
					"0123456789ABCDEF";

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
			err = _yIO_vec_putc(v, '0');
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
			err = _yIO_vec_putc(v, '0');
			if (err) return err;
		} else {
			err = get_next_digit$1(v, &val, hex, to_digit_str, precision == 0);
			if (err) return err;
		}
	} else if (speclower == 'a') {
		err = _yIO_vec_putc(v, '0');
		if (err) return err;
		err = _yIO_vec_putc(v, is_lower_spec ? 'x': 'X');
		if (err) return err;
		if (val_is_zero) {
			err = _yIO_vec_putc(v, '0');
			if (err) return err;
		} else {
			// print first number
			if (val < 2) {
				err = _yIO_vec_putc(v, '1');
				if (err) return err;
				val -= 1;
			} else {
				// it rounded to whole number up
				err = _yIO_vec_putc(v, '2');
				if (err) return err;
				// print only zeros below
				val = 0;
			}
		}
	}

	bool g_fractional_part_removed = false;
	if (precision) {
		err = _yIO_vec_putc(v, '.');
		if (err) return err;
		int zeros = (speclower == 'f' && exponent < 0) ? -exponent : 0;
		while (precision--) {
			if (zeros) {
				--zeros;
				err = _yIO_vec_putc(v, '0');
				if (err) return err;
			} else {
				err = get_next_digit$1(v, &val, hex, to_digit_str, precision == 0);
				if (err) return err;
			}
		}
		if (spec0lower == 'g') {
			g_fractional_part_removed = _yIO_vec_remove_trailing_zeros(v);
		}
	}

	const bool print_scientif_suffix = scientific &&
			!(spec0lower == 'g' && !val_is_zero && exponent == 1 && !g_fractional_part_removed);
	if (print_scientif_suffix) {
		const char letter = !hex ? spec :
				is_lower_spec ? 'p' : 'P';
		err = _yIO_vec_putc(v, letter);
		if (err) return err;
		err = _yIO_vec_yreformatf(v, "{:+0{}}", hex ? 0 : 3, val_is_zero ? 0 : (exponent - 1));
		if (err) return err;
	}

	SUCCESS:
	err = _yIO_vec_putc(v, '\0');
	if (err) return err;
	*out = _yIO_vec_data(v);
	return _yIO_vec_size(v);
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
