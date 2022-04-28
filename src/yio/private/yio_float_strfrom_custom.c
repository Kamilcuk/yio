/**
 * @file
 * @date 2020-06-12
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
#include "yio_float_strfrom_custom.h"
#include "yio_res.h"
#include "yio_float.h"
#include "private.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NDEBUG
#define ASSERTMSG(expr, fmt, ...) do { \
	if (!(expr)) { \
		fprintf(stderr, "%s:%d: ERROR: %s failed: " fmt "\n", \
				__func__, __LINE__, #expr, ##__VA_ARGS__); \
		void abort(void); \
		abort(); \
	} \
} while (0)
#else
#define ASSERTMSG(...)  ((void)0)
#endif


static const char _yIO_NAN[3] = {'N','A','N'};
static const char _yIO_nan[3] = {'n','a','n'};
static const char (*_yIO_nans[3])[] = { &_yIO_NAN, &_yIO_nan, };
static const char _yIO_INF[3] = {'I','N','F'};
static const char _yIO_inf[3] = {'i','n','f'};
static const char (*_yIO_infs[3])[] = { &_yIO_INF, &_yIO_inf, };
static const char _yIO_digit_to_HEX[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static const char _yIO_digit_to_hex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
static const char (*_yIO_digit_to_hexs[16])[] = { &_yIO_digit_to_HEX, &_yIO_digit_to_hex, };

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
	{% if not j_match(V.0, "^d[0-9]") %}{# exclude floats #}
#line
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

#define TYPE     _yIO_FLOAT$1
#define FLOOR    _yIO_floor$1
#define EXP2     _yIO_exp2$1
#define EXP10    _yIO_exp10$1
#define FABS     _yIO_fabs$1
#define FREXP2   _yIO_frexp2$1
#define FREXP10  _yIO_frexp10$1
#define FC(x)    _yIO_FLOAT_C$1(x)

{% if j_match(V.0, "^d[0-9]") %}{# check if DECIMAL float #}
#line
#if __GNUC__
// Workaround for GNU bug aroud decimal floating point numbers.
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=102674
#ifndef DEC_INFINITY
#define DEC_INFINITY  __builtin_inf$1()
#endif
#ifndef DEC_NAN
#define DEC_NAN  __builtin_nan$1("")
#endif
#define ISINF(x)  (x == DEC_INFINITY || x == -DEC_INFINITY)
#define FPCLASSIFY(x)  (ISINF(x) ? FP_INFINITE : x != x ? DEC_NAN : x == 0 ? FP_ZERO : FP_NORMAL)
#endif
{% else %}
#line
#define FPCLASSIFY fpclassify
#define ISINF  isinf
{% endif %}
#line

static inline
int get_next_digit$1(_yIO_res *v, TYPE *val,
		bool dec, const char *to_digit_str, bool is_last) {
	*val = dec ? (*val * FC(10.0)) : (*val * FC(16.0));

	const int digit = *val;

	const int baseint = dec ? 10 : 16;
	ASSERTMSG(0 <= digit && digit < baseint,
			"digit=%d dec=%d is_last=%d val=%a\n",
			digit, dec, is_last, (float)*val
	);
	if (!(0 <= digit && digit <= baseint)) {
		return YIO_ERROR_ENOSYS;
	}

	const char c = to_digit_str[digit];
	const int err = _yIO_res_putc(v, c);
	if (err) return err;
	if (!is_last) {
		*val -= digit;
	}
	return 0;
}

int _yIO_float_astrfrom_custom$1(_yIO_res *v, const int precision0, const char spec0, TYPE val) {
	static const int a_max_precision =
#if FLT_RADIX == 2
// if the precision is missing and FLT_RADIX is a power of 2,
// then the precision is sufficient for an exact representation of the value
			_yIO_FLOAT_MANT_DIG$1 / 4 + !!(_yIO_FLOAT_MANT_DIG$1 % 4);
#else
// if the precision is missing and FLT_RADIX is not a power of 2,
// then the precision is sufficient to distinguish values of type double,
// except that trailing zeros may be omitted
// Will this code ever execute?
			round(log2(exp(FLT_RADIX, _yIO_FLOAT_MANT_DIG$1)) + 0.5);
#endif

	int err = 0;

	// take minus out of the way
	const bool negative = signbit(val);
	if (negative) {
		err = _yIO_res_putc(v, '-');
		if (err) return err;
		val = FABS(val);
	}

	const char spec0lower = tolower(spec0);
	const bool is_lower_spec = spec0lower == spec0;

	// take INF and NAN out of the way
	const int val_class = FPCLASSIFY(val);
	const char (*nan_or_inf_str)[3] =
			val_class == FP_NAN ? _yIO_nans[is_lower_spec] :
					val_class == FP_INFINITE ? _yIO_infs[is_lower_spec] :
							NULL;
	if (nan_or_inf_str != NULL) {
		err = _yIO_res_putsn(v, *nan_or_inf_str, 3);
		if (err) return err;
		goto SUCCESS;
	}

	// All the happy rest.
	// fF 987.654
	// eE 9.87654e+02
	// aA 1.<hex>p+1
	// gG if <exponent> then f else e

	const bool val_is_zero = val_class == FP_ZERO;
	// These change after they are determined.
	char spec = spec0;
	char speclower = spec0lower;
	int precision = precision0;
	// This is exponent for number between (1/radix)<=x<1.0.
	// The printed exponent is one less, cause of the initial digit!
	int exponent;

	if (precision0 < 0) {
		if (spec0lower == 'a') {
			precision = a_max_precision;
		} else {
			precision = 6;
		}
	}

	// Rounding with 'e' specifier is shared with 'g' in case it get's chosen
	int exponent10 = 0;
	TYPE val10 = val;
	if ((spec0lower == 'g' || spec0lower == 'e') && !val_is_zero) {
		FREXP10(val, &exponent10);
		const int round_exp10 = exponent10 -
				// this is strange, the standard says "Let P" not "Let precision".
				((spec0lower == 'g' && precision == 0) ? 1 : precision);
		val10 = val + (spec0lower == 'g' ? FC(0.5) : FC(0.05)) * EXP10(round_exp10);
		if (ISINF(val10)) {
			// We can't round up - stay as it is.
			val10 = val;
		}
		val10 = FREXP10(val10, &exponent10);
	}

	// Let P equal the precision if nonzero, 6 if the precision is omitted,
	// or 1 if the precision is zero.
	// Then, if a conversion with style E would have an exponent of X:
	// if P > X >= -4, the conversion is with style f (or F) and precision P - (X + 1).
	// otherwise, the conversion is with style e (or E) and precision P - 1.
	if (spec0lower == 'g') {
		const int P = precision == 0 ? 1 : precision;
		const int X = exponent10 - 1;
		if (P > X && X >= -4) {
			spec = is_lower_spec ? 'f' : 'F';
			speclower = 'f';
			precision = P - exponent10; /* + 1 - 1 */
		} else {
			spec = is_lower_spec ? 'e' : 'E';
			speclower = 'e';
			precision = P - 1;
		}
	}

	assert(precision >= 0);

	// Extract exponent and round the number
	if (val_is_zero) {
		exponent = 0;
	} else if (speclower == 'f') {
		const _yIO_FLOAT$1 tmp = val + FC(0.5) * EXP10(-precision);
		if (!ISINF(tmp)) {
			val = tmp;
		}
		val = FREXP10(val, &exponent);
	} else if (speclower == 'a') {
		// rounding makes no sense, when precision is maximum available
		if (precision0 >= 0) {
			int exponent_tmp;
			FREXP2(val, &exponent_tmp);
			if (precision > INT_MAX / 4) {
				return YIO_ERROR_ENOSYS;
			}
			const int bitpos = -5 + -4 * precision + exponent_tmp;
			_yIO_FLOAT$1 tmp = val + EXP2(bitpos);
			if (!ISINF(tmp)) {
				val = tmp;
			}
		}
		val = FREXP2(val, &exponent);
		// I start printing with initial leading 1 bit always set.
		exponent -= 3;
	} else if (speclower == 'e') {
		// rounded above
		exponent = exponent10;
		val = val10;
	} else {
		assert(0);
		return YIO_ERROR_FMT_INVALID;
	}

	// at this point, val should be after frexp
	assert(0 <= val);
	assert(val < 1);

	const bool dec = speclower != 'a';
	const char * const to_digit_str = *_yIO_digit_to_hexs[is_lower_spec];

	// Convert number before the dot
	if (speclower == 'f') {
		if (exponent <= 0) {
			err = _yIO_res_putc(v, '0');
			if (err) return err;
		} else {
			assert(exponent > 0);
			for (int i = exponent; i; --i) {
				err = get_next_digit$1(v, &val, dec, to_digit_str, i == 1 && precision == 0);
				if (err) return err;
			}
		}
	} else if (speclower == 'e') {
		if (val_is_zero) {
			err = _yIO_res_putc(v, '0');
			if (err) return err;
		} else {
			err = get_next_digit$1(v, &val, dec, to_digit_str, precision == 0);
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
			err = get_next_digit$1(v, &val, dec, to_digit_str, precision == 0);
			if (err) return err;
		}
	}

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
				err = get_next_digit$1(v, &val, dec, to_digit_str, precision == 0);
				if (err) return err;
			}
		}
		if (spec0lower == 'g' || spec0lower == 'a') {
			_yIO_res_remove_trailing_zeros_and_comma(v);
		}
	}

	const bool print_scientific_suffix = speclower == 'e' || speclower == 'a';
	if (print_scientific_suffix) {
		assert(strchr("eEaA", spec) != NULL);
		const char letter = dec ? spec : is_lower_spec ? 'p' : 'P';
		err = _yIO_res_putc(v, letter);
		if (err) return err;
		err = _yIO_res_yprintf(v, "{:+0{}}", val_is_zero ? 0 : (exponent - 1), dec ? 3 : 0);
		if (err) return err;
	}

	SUCCESS:
	return 0;
}

#undef TYPE
#undef FLOOR
#undef EXP2
#undef EXP10
#undef FABS
#undef FREXP2
#undef FREXP10
#undef FC
#undef FPCLASSIFY
#undef ISINF

#endif

{% endif %}{% endcall %}
