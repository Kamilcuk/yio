/**
 * @file
 * @date 2020-sie-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "yio_res.h"
#include "yio_stdfix.h"
#include "yio_stdfix_strfrom.h"
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>

#if _yIO_HAS_STDFIX_TYPES

{% from 'yio/private/yio_stdfix.h' import j_STDFIX %}

static const char _yIO_stdfix_strfrom_i_to_c_HEX[] = "0123456789ABCDEF";
static const char _yIO_stdfix_strfrom_i_to_c_hex[] = "0123456789abcdef";
static inline
const char *_yIO_stdfix_strfrom_i_to_c(bool upper) {
	return upper ? _yIO_stdfix_strfrom_i_to_c_HEX : _yIO_stdfix_strfrom_i_to_c_hex;
}

{% call j_FOREACHAPPLY([8, 16, 32, 64]) %}
#line
#define TYPEUINT  uint_least$1_t

// I think this is countl, but I am not sure
static inline
int _yIO_stdfix_strfrom_int$1_ffs(TYPEUINT tmp) {
	int exp;
	for (exp = 0; exp < (int)sizeof(tmp) * CHAR_BIT; ++exp) {
		if (!(tmp & 1)) {
			break;
		}
		tmp >>= 1;
	}
	return exp;
}

static inline
int _yIO_stdfix_strfrom_int$1(int precision0, int precision, char spec, bool spec_is_upper, _yIO_res *o, TYPEUINT v, int ibit, int fbit) {
	int err = 0;
	(void)err;
	const char * const i_to_c = _yIO_stdfix_strfrom_i_to_c(spec_is_upper);

	if (spec == 'x') {
		err = _yIO_res_yprintf(o, "{:x}", v + 0);
		if (err) return err;
	} else if (spec == 'f') {
		const bool only_fract = fbit == sizeof(v) * CHAR_BIT;

		if (only_fract) {
			// it's all fractional anyway, just zero
			err = _yIO_res_putc(o, '0');
			if (err) return err;
		} else {
			// a little bit of a hack to print it on newlib-nano
			// fbit is going to be max 32 anyway
			typedef m4_ifelse(«$1», «64», «uint32_t», «uint$1_t») uint_32_when_64_otherwise_type_t;
			const uint_32_when_64_otherwise_type_t integer_part = v >> fbit;
			// The + 0 promotes integer_part to an integer type, so it isn't detected as (char)
			err = _yIO_res_yprintf(o, "{:d}", integer_part + 0);
			if (err) return err;
		}
		err = _yIO_res_putc(o, '.');
		if (err) return err;

		TYPEUINT num = v;
		// move ibit and signbit to the right so that the high bit of fract is the first bit
		num <<= (sizeof(v) * CHAR_BIT - fbit);

		for (int k = 0; k < precision; ++k) {
			const TYPEUINT dividend = (UINT$1_MAX / 10);
			const int digit = num / dividend;
			num %= dividend;
			num *= 10;

			assert(0 <= digit && digit < 10);
			const char c = i_to_c[digit];
			err = _yIO_res_putc(o, c);
			if (err) return err;
		}
	} else if (spec == 'a') {
		err = _yIO_res_putc(o, '0');
		if (err) return err;
		err = _yIO_res_putc(o, spec_is_upper ? 'X' : 'x');

		int exponent = 0;
		if (v != 0) {
			while (!(v >> (sizeof(v) * CHAR_BIT - 1))) {
				v <<= 1;
				--exponent;
			}
			precision = precision0 < 0 ? ((int)sizeof(v) * CHAR_BIT / 4 - 1) : precision0;
			exponent += ibit - 4;
		} else {
			precision = precision0 < 0 ? 0 : precision0;
		}

		const int digits = precision + 1;

		// rounding
		const int roundbitpos = fbit - digits * 4 - 1;
		if (roundbitpos > 0) {
			v += 1 << roundbitpos;
		}

		for (int i = 0; i < digits; ++i) {
			if (precision0 < 0 && v == 0 && i > 0) {
				break;
			}
			const int c = v >> (sizeof(v) * CHAR_BIT - 4);
			v <<= 4;
			assert(0 <= c && c < 16);
			if (i == 1) {
				err = _yIO_res_putc(o, '.');
				if (err) return err;
			}
			err = _yIO_res_putc(o, i_to_c[c]);
			if (err) return err;
		}

		err = _yIO_res_putc(o, spec_is_upper ? 'P' : 'p');
		if (err) return err;

		err = _yIO_res_yprintf(o, "{:+d}", exponent + 0);
		if (err) return err;
	} else {
		_yIO_res_end_err(o);
		return YIO_ERROR_ENOSYS;
	}

	return err;
}

#undef TYPEUINT
{% endcall %}


{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
#line
#define TYPE      $2
#define TYPEUINT  _yIO_UINT_$3

// Many things are missing here
int _yIO_astrfrom$1(char **resultp, size_t *lengthp, int precision0, char spec0, TYPE val) {
	_Static_assert(sizeof(val) <= sizeof(TYPEUINT), "");

	int err = 0;
	(void)err;

	const char spec = spec0 ? tolower(spec0) : 'f';
	const bool spec_is_upper = isupper(spec0);

	_yIO_res _mem;
	_yIO_res *o = &_mem;
	_yIO_res_init(o, resultp, lengthp);

	{% if not j_match(V.1, "unsigned") %}
	#line
	// Handle negative for x type for signed.
	if (spec != 'x') {
		const bool negative = val < 0;
		if (negative) {
			err = _yIO_res_putc(o, '-');
			if (err) return err;
			// TODO: undefined behavior for _MIN
			val = -val;
		}
	}
	{% endif %}
	#line

	int precision = 0;
	// rounding
	if (spec == 'f') {
		precision = precision0 <= 0 ? 6 : precision0;
		TYPE ro = 0.5;
		for (int i = 0; i < precision; ++i) {
			ro /= 10;
		}
		val += ro;
	}

	// we are "dispatching" each size to the same size of variable
	// cause after removing negative numbers and in twos-complement representation
	// we do not really care
	TYPEUINT v = 0;
	memcpy(&v, &val, sizeof(v));
#if _yIO_BITS_$3 == 8
#define _yIO_stdfix_strfrom_intX _yIO_stdfix_strfrom_int8
#elif _yIO_BITS_$3 == 16
#define _yIO_stdfix_strfrom_intX _yIO_stdfix_strfrom_int16
#elif _yIO_BITS_$3 == 32
#define _yIO_stdfix_strfrom_intX _yIO_stdfix_strfrom_int32
#elif _yIO_BITS_$3 == 64
#define _yIO_stdfix_strfrom_intX _yIO_stdfix_strfrom_int64
#else
#error
#endif
	err = _yIO_stdfix_strfrom_intX(precision0, precision, spec, spec_is_upper, o, v, _yIO_$3_IBIT, _yIO_$3_FBIT);
#undef _yIO_stdfix_strfrom_intX
	if (err) return err;

	_yIO_res_end(o, resultp, lengthp);
	return 0;
}

#undef TYPE
#undef TYPEUINT
{% endcall %}

#endif // _yIO_HAS_STDFIX_TYPES
