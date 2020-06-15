/**
 * @file yio_float_strfrom_stupid.c
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE
m4_config();
#include "yio_float_strfrom_stupid.h"
#include "yio_float_string.h"
#include "yio_buf.h"
#include "yio_float.h"
#include <yio.h>

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#define ASSERTMSG(expr, ...) do { \
	if (!(expr)) { \
		fprintf(stderr, __VA_ARGS__); \
		__assert_fail(#expr, __FILE__, __LINE__, __func__); \
	} \
} while (0)


m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

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
void get_next_digit$1(_yIO_buf *b, TYPE *val,
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
	_yIO_buf_putc(b, c);
	if (!is_last) {
		*val -= digit;
	}
}

int _yIO_float_strfrom_stupid$1(char *dest, int precision, char spec0, TYPE val) {
	char spec = spec0;
	_yIO_buf b = _yIO_buf_init(dest);

	// take minus out of the way
	const bool negative = signbit(val);
	if (negative) {
		_yIO_buf_putc(&b, '0');
		val = FABS(val);
	}

	// take INF and NAN out of the way
	const bool upper = isupper((unsigned char)spec);
	const char *nan_or_inf_str =
			isnan(val) ? upper ? "NAN" : "nan" :
					isinf(val) ? upper ? "INF" : "inf" : NULL;
	if (nan_or_inf_str) {
		_yIO_buf_puts(&b, nan_or_inf_str);
		return _yIO_buf_end(&b);
	}

	// all the happy rest

	// fF 987.654
	// eE 9.87654e+02
	// aA 1.<hex>p+1
	// gG if <exponent> then f else e
	char speclower = tolower(spec);
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
			_yIO_buf_putc(&b, '0');
		} else {
			assert(exponent > 0);
			for (int i = exponent; i; --i) {
				get_next_digit$1(&b, &val, hex, to_digit_str, i == 1 && precision == 0);
			}
		}
	} else if (speclower == 'e') {
		if (val_is_zero) {
			_yIO_buf_putc(&b, '0');
		} else {
			get_next_digit$1(&b, &val, hex, to_digit_str, precision == 0);
		}
	} else if (speclower == 'a') {
		_yIO_buf_putc(&b, '0');
		_yIO_buf_putc(&b, is_lower_spec ? 'x': 'X');
		if (val_is_zero) {
			_yIO_buf_putc(&b, '0');
		} else {
			// print first number
			if (val < 2) {
				_yIO_buf_putc(&b, '1');
				val -= 1;
			} else {
				// it rounded to whole number up
				_yIO_buf_putc(&b, '2');
				// print only zeros below
				val = 0;
			}
		}
	}

	bool g_fractional_part_removed = false;
	if (precision) {
		_yIO_buf_putc(&b, '.');
		int zeros = (speclower == 'f' && exponent < 0) ? -exponent : 0;
		while (precision--) {
			if (zeros) {
				--zeros;
				_yIO_buf_putc(&b, '0');
			} else {
				get_next_digit$1(&b, &val, hex, to_digit_str, precision == 0);
			}
		}
		if (spec0 == 'g' || spec0 == 'G') {
			g_fractional_part_removed = _yIO_buf_remove_trailing_zeros(&b);
		}
	}

	const bool print_scientif_suffix = scientific &&
			!((spec0 == 'g' || spec0 == 'G') &&
					!val_is_zero && exponent == 1 && !g_fractional_part_removed);
	if (print_scientif_suffix) {
		const char letter = !hex ? spec :
				is_lower_spec ? 'p' : 'P';
		_yIO_buf_putc(&b, letter);
		int len = ysprintf(b.dest,
				b.dest == NULL ? 0 : SIZE_MAX,
				"{:+0{}}",
				hex ? 0 : 3,
				val_is_zero ? 0 : (exponent - 1));
		b.len += len;
		if (b.dest != NULL) {
			b.dest += len;
		}
	}

	return _yIO_buf_end(&b);
}

int _yIO_float_astrfrom_stupid$1(char **out, int precision, char type, TYPE val) {
	return _yIO_float_strfrom_to_astrfrom$1(out, precision, type, val, _yIO_float_strfrom_stupid$1);
}

#undef TYPE
#undef PRI
#undef FLOOR
#undef EXP2
#undef EXP10
#undef FABS
#undef FREXP2
#undef FREXP10

~)
