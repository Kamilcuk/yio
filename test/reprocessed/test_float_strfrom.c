#define _GNU_SOURCE  1
#define _ISOC99_SOURCE  1
#include <yio_test_private.h>
#include <yio_test_float.h>
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

static inline
bool get_only_last_char_differs(const char *buf, const char *valstr) {
	if (!(strlen(buf) == strlen(valstr))) {
		return false;
	}
	if (!(memcmp(buf, valstr, strlen(buf)) == 0)) {
		return false;
	}
	const char digitsdot[] = "0123456789.";
	const char *buf_last_digit = strpbrk(buf, digitsdot);
	const char *valstr_last_digit = strpbrk(valstr, digitsdot);
	assert(buf_last_digit != NULL);
	assert(valstr_last_digit != NULL);
	// printf("%s %s\n", buf_last_digit, valstr_last_digit);
	return abs(*buf_last_digit - *valstr_last_digit) <= 1;
}

{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

static int _yIO_test_print_float_custom_in$1(int precision,
        char type, _yIO_FLOAT$1 val, const char *valstr0,
		int (*astrfrom)(_yIO_res *res, int precision, char type, _yIO_FLOAT$1 val),
		const char *astrfrom_str) {
	_yIO_res res;
	_yIO_res_init(&res, 0, 0);
	int err = astrfrom(&res, precision, type, val);
	if (err) {
		_yIO_TEST(err == 0, "%s(%d, %c, %s, %s) failed -> %d",
				__func__, precision, type, valstr0, astrfrom_str, err);
		return err;
	}
	// zero terminate result
	err = _yIO_res_putc(&res, '\0');
	if (err) return err;

	char *valstr = NULL;
	if (precision < 0) {
		char *fmt = NULL;
		err = asprintf(&fmt, "%%" _yIO_FLOAT_PRI$1 "%c", type);
		_yIO_TEST(err > 0, "asprintf(&fmt, ...) failed err=%d", err);
		err = asprintf(&valstr, fmt, val);
		_yIO_TEST(err > 0, "asprintf(&valstr, ...) failed err=%d", err);
		free(fmt);
	} else {
		char *fmt = NULL;
		err = asprintf(&fmt, "%%.*" _yIO_FLOAT_PRI$1 "%c", type);
		_yIO_TEST(err > 0, "asprintf(&fmt, ...) failed err=%d", err);
		err = asprintf(&valstr, fmt, precision, val);
		_yIO_TEST(err > 0, "asprintf(&valstr, ...) failed err=%d", err);
		free(fmt);
	}

	const char *const result = _yIO_res_begin(&res);
	const bool differ = strcmp(result, valstr) != 0;
	if (differ) {
		const bool only_last_char_differs = get_only_last_char_differs(result, valstr);
		printf("%2s(%d,%c,%s%s%s%.30"_yIO_FLOAT_PRI$1"g,%s): %s != %s %s%s\n",
				// <
				"$1",
				// >(
				precision, type,
				valstr0 ? "\"" : "",
				valstr0 ? valstr0 : "",
				valstr0 ? "\"=" : "",
				val,
				astrfrom_str,
				// ):
				result,
				// =
				valstr,
				// ' '
				differ ? "__DIFFER__" : "",
				only_last_char_differs ? "ONLY_LAST" : ""
		);

		bool workaround = false;
		// for long double and 'a', we just assume it works
		// sadly, glibc chooses different exponents
		if('$1' == 'l' &&
				strcmp(astrfrom_str, "_yIO_float_astrfrom_customl") == 0 &&
				(type == 'A' || type == 'a')) {
			workaround = true;
		}
		if (!only_last_char_differs && !workaround) {
			err = __LINE__;
		} else {
			err = 0;
		}
	}
	free(valstr);
	_yIO_res_end(&res);

	return err;
}

static void _yIO_run_tests_print_float_custom$1(void) {
	static const char specs[] = {
			'F',
			'E',
			'G',
			'f',
			'e',
			'g',
	};
	static const int precisions[] = {
			-1, 0,
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	};

	for (size_t istrfrom = 0; istrfrom < ARRAY_SIZE(_yIO_astrfroms$1); ++istrfrom) {
		for (size_t ispec = 0; ispec < ARRAY_SIZE(specs); ++ispec) {
			for (size_t ival = 12; ival < ARRAY_SIZE(_yIO_test_floatlist$1); ++ival) {
				for (size_t iprec = 0; iprec < ARRAY_SIZE(precisions); ++iprec) {
					_yIO_test_print_float_custom_in$1(
							precisions[iprec],
							specs[ispec],
							_yIO_test_floatlist$1[ival].val,
							_yIO_test_floatlist$1[ival].valstr,
							_yIO_astrfroms$1[istrfrom].astrfrom,
							_yIO_astrfroms$1[istrfrom].astrfrom_str
					);
				}
			}
		}
	}
}

#endif

{% endcall %}

int main() {
#ifdef __GLIBC__
	_yIO_run_tests_print_float_customf();
	_yIO_run_tests_print_float_customd();
	if (!_yIO_test_is_in_valgrind())  {
		_yIO_run_tests_print_float_customl();
	}
#endif
	return 0;
}

