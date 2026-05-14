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
#include <string.h>
#include <assert.h>
#include <errno.h>

static bool verbose = 0;

{% call j_FOREACHAPPLY([
		["f", "float",  ""],
		["d", "double", ""],
		["l", "long double", "L"],
	]) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

static int YIO_test_print_float_naive_in$1(int precision0,
        char type, YIO_FLOAT$1 val, const char *valstr0,
		int (*astrfrom)(YIO_string *res, int precision0, char type, YIO_FLOAT$1 val),
		const char *astrfrom_str) {
	YIO_string res; YIO_string_init(&res);
	int err = astrfrom(&res, precision0, type, val);
	if (err) {
		YIO_TESTEXPR(err == 0, "%s(%d, %c, %s, %s) failed -> %d",
				__func__, precision0, type, valstr0, astrfrom_str, err);
		YIO_string_fini(&res);
		return err;
	}
	// zero terminate result
	err = YIO_string_putc(&res, '\0');
	if (err) { YIO_string_fini(&res); return err; }

	const char *result = YIO_string_data(&res);

	char valstr[1024];
	char format[128];
	if (precision0 >= 0) {
		snprintf(format, sizeof(format), "%%.%d$3%c", precision0, type);
	} else {
		snprintf(format, sizeof(format), "%%$3%c", type);
	}
	snprintf(valstr, sizeof(valstr), format, val);

	if (strcmp(result, valstr) != 0) {
		bool only_last_char_differs = false;
		if (strlen(result) == strlen(valstr) && strlen(result) > 0) {
			only_last_char_differs = true;
			for (size_t i = 0; i < strlen(result) - 1; ++i) {
				if (result[i] != valstr[i]) {
					only_last_char_differs = false;
					break;
				}
			}
		}

		if (verbose) {
			printf("%s(%d, %c, %s%s%s%g, %s): '%s' != '%s' %s\n",
				__func__,
				precision0, type,
				valstr0 ? "\"" : "",
				valstr0 ? valstr0 : "",
				valstr0 ? "\"=" : "",
				(double)val,
				astrfrom_str,
				// ):
				result,
				// =
				valstr,
				// ' '
				only_last_char_differs ? "(ONLY LAST CHAR DIFFERS)" : ""
			);
		}

		bool workaround = false;
		// sadly, glibc chooses different exponents for 'a'/'A',
		// and naive implementation has limited precision
		if (strstr(astrfrom_str, "YIO_float_astrfrom_naive") != NULL) {
			workaround = true;
		}

		bool match = YIO_test_float_equal(result, valstr, type);
		if (!only_last_char_differs && !match && !workaround) {
			err = __LINE__;
			YIO_TESTEXPR(err == 0, "%s(%d, %c, %s, %s): '%s' != '%s'",
					__func__, precision0, type, valstr0, astrfrom_str, result, valstr);
		}
	}

	YIO_string_fini(&res);
	return err;
}

static void YIO_run_tests_print_float_naive$1(void) {
	static const char specs[] = { 'f', 'F', 'e', 'E', 'g', 'G', 'a', 'A' };
	static const int precisions[] = {
			-1, // unset
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, // 0..10
	};

	for (size_t iastrfrom = 0; YIO_astrfroms$1[iastrfrom].astrfrom != NULL; ++iastrfrom) {
		for (size_t ispec = 0; ispec < ARRAY_SIZE(specs); ++ispec) {
			for (size_t ival = 12; ival < ARRAY_SIZE(YIO_test_floatlist$1); ++ival) {
				for (size_t iprec = 0; iprec < ARRAY_SIZE(precisions); ++iprec) {
					YIO_test_print_float_naive_in$1(
							precisions[iprec],
							specs[ispec],
							YIO_test_floatlist$1[ival].val,
							YIO_test_floatlist$1[ival].valstr,
							YIO_astrfroms$1[iastrfrom].astrfrom,
							YIO_astrfroms$1[iastrfrom].astrfrom_str
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
	YIO_run_tests_print_float_naivef();
	YIO_run_tests_print_float_naived();
	if (!YIO_test_is_in_valgrind())  {
		YIO_run_tests_print_float_naivel();
	}
#endif
	return 0;
}
