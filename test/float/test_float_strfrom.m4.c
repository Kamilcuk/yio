#define _GNU_SOURCE  1
#define _ISOC99_SOURCE  1
#include <yio/private/yio_float.h>
#include <yio/private/yio_float_strfrom_printf.h>
#include <yio/private/yio_float_strfrom_strfrom.h>
#include <yio/private/yio_float_strfrom_stupid.h>
#include <yio_test.h>
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
#define STRING(a) #a
#define XSTRING(a) STRING(a)

m4_applyforeachdefine(`((f,f,F), (,d,), (l,l,L))~, `m4_dnl;

int _yIO_test_print_float_stupid_in$1(int precision,
        char type, _yIO_FLOAT$1 val, const char *valstr0,
		int (*astrfrom)(char **out, int precision, char type, _yIO_FLOAT$1 val),
		const char *astrfrom_str) {
	char *buf;

	int err = astrfrom(&buf, precision, type, val);
	if (err) {
		_yIO_TEST(err == 0, "%s(%d, %c, %s, %s) failed -> %d",
				__func__, precision, type, valstr0, astrfrom_str, err);
		return err;
	}

	char *valstr = NULL;
	if (precision < 0) {
		err = asprintf(&valstr, (char[]){'%',type,'\0'}, val);
		_yIO_TEST(err > 0, "asprintf(&valstr, type, ...) failed err=%d", err);
	} else {
		char *fmt = NULL;
		err = asprintf(&fmt, "%%.*" _yIO_FLOAT_PRI$1 "%c", type);
		_yIO_TEST(err > 0, "asprintf(&fmt, ...) failed err=%d", err);
		err = asprintf(&valstr, fmt, precision, val);
		_yIO_TEST(err > 0, "asprintf(&valstr, ...) failed err=%d", err);
		free(fmt);
	}

	const bool differ = strcmp(buf, valstr) != 0;
	if (differ) {
		const bool only_last_char_differs = get_only_last_char_differs(buf, valstr);
		printf("  test_print_float<%s(%s)>(%d,%c,%s%s%s%.30"_yIO_FLOAT_PRI$1"g,%s): %s != %s %s%s\n",
				// <
				XSTRING(_yIO_FLOAT$1),
				_yIO_FLOAT_PRI$1,
				// >(
				precision, type,
				valstr0 ? "\"" : "",
				valstr0 ? valstr0 : "",
				valstr0 ? "\"=" : "",
				val,
				astrfrom_str,
				// ):
				buf,
				// =
				valstr,
				// ' '
				differ ? "__DIFFER__" : "",
				only_last_char_differs ? "ONLY_LAST" : ""
		);

		bool workaround = false;
		// for long double and 'a', we just assume it works
		// sadly, glibc chooses different exponents
		if('$2' == 'l' &&
				strcmp(astrfrom_str, "_yIO_float_astrfrom_stupidl") == 0 &&
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
	free(buf);

	return err;
}

void _yIO_run_tests_print_float_stupid$1(void) {
	struct precision_val_s {
		int precision;
		_yIO_FLOAT$1 val;
		const char *valstr;
	};
	const struct precision_val_s tests[] = {
#define T(a, b)  { a, b, #b, }
			T(0, 0.0),
			T(0, 0.1),
			T(0, 0.2),
			T(0, 0.3),
			T(0, 0.4),
			T(0, 0.5),
			T(0, 0.6),
			T(0, 0.7),
			T(0, 0.8),
			T(0, 0.9),
			T(0, 1.0),
			T(0, 2.0),
			T(0, 3.0),
			T(0, 4.0),
			T(0, 5.0),
			T(0, 6.0),
			T(0, 7.0),
			T(0, 8.0),
			T(0, 9.0),
			T(1, 0.01),
			T(1, 0.02),
			T(1, 0.03),
			T(1, 0.04),
			T(1, 0.05),
			T(1, 0.06),
			T(1, 0.07),
			T(1, 0.08),
			T(1, 0.09),
			T(6, INFINITY),
			T(6, HUGE_VAL$3),
			T(6, NAN),
			T(0, 123.456),
			T(6, 123.456),
			T(6, 1111111111.99),
			T(6, 9234567890.123),
			T(5, 9234569999.99),
			T(6, 0e-200),
			T(6, 0.9123456789),
			T(6, 123.456),
			T(0, 1111111111.99),
			T(0, 9234567890.123),
			T(0, 9234569999.99),
			T(0, 0e-200),
			T(0, 0.9123456789),
			T(2, 0x1.ffep-2),
			T(2, 0x1.fffffffffffffep-2),
			T(0, 1.49),
			T(0, 2.49),
			T(0, 3.49),
			T(0, 4.49),
			T(0, 5.49),
			T(0, 6.49),
			T(0, 7.49),
			T(0, 8.49),
			T(0, 9.49),
			T(0, 1.51),
			T(0, 2.51),
			T(0, 3.51),
			T(0, 4.51),
			T(0, 5.51),
			T(0, 6.51),
			T(0, 7.51),
			T(0, 8.51),
			T(0, 9.51),
			T(0, 10),
			T(0, 9.5),
			T(2, 9.555555),
			T(3, 9.555555),
			T(4, 9.555555),
			T(5, 9.555555),
			T(6, 9.555555),
#undef T
	};

	struct astrfrom_and_str_s {
		int (*astrfrom)(char **out, int precision, char type, _yIO_FLOAT$1 val);
		const char *astrfrom_str;
	};
	struct astrfrom_and_str_s astrfrom_and_str_arr[] = {
#define T(a)  { a, #a, }
#ifdef _yIO_HAVE_strfrom$2
			T(_yIO_float_astrfrom_strfrom$1),
#endif
			T(_yIO_float_astrfrom_stupid$1),
			T(_yIO_float_astrfrom_printf$1)
#undef T
	};

	const char specs[] = {
		'f', 'F',
		'a', 'A',
		'e', 'E',
		'g', 'G',
	};

#define ARRAY_SIZE(a) sizeof(a)/sizeof(*a)
#define ARRAY_END(a)  (a + ARRAY_SIZE(a))

	for (const struct astrfrom_and_str_s *iastrfrom_and_str = astrfrom_and_str_arr;
			iastrfrom_and_str != ARRAY_END(astrfrom_and_str_arr);
			++iastrfrom_and_str) {
		for (const char *ispec = specs;
				ispec != ARRAY_END(specs);
				++ispec) {
			for (const struct precision_val_s *itest = tests;
					itest != ARRAY_END(tests);
					++itest) {

				_yIO_test_print_float_stupid_in$1(
						itest->precision,
						*ispec,
						itest->val,
						itest->valstr,
						iastrfrom_and_str->astrfrom,
						iastrfrom_and_str->astrfrom_str
				);

			}
		}
	}
}

~) m4_dnl;

static inline
int tests_run_within_valgrind(void) {
#if __linux__
	// long double on x86_64 are not supported on valgrind
	const char *p = getenv("LD_PRELOAD");
	if (p == NULL) return 0;
	return strstr (p, "/valgrind/") != NULL ||
			strstr (p, "/vgpreload") != NULL;
#else
	return false;
#endif
}

int main() {
	_yIO_run_tests_print_float_stupidf();
	_yIO_run_tests_print_float_stupid();
	if (!tests_run_within_valgrind())  {
		_yIO_run_tests_print_float_stupidl();
	}
	return 0;
}

