/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_syncline(1)m4_dnl;
#include <yio_test.h>
#include <math.h>
#include <assert.h>
#include <yio_test_float.h>

static bool verbose = 0;

struct formats_s {
	double diffatmost;
	const char *const format;
};

static const struct formats_s formats[] = {
		{ 1e-5, "{}" },

		{ 1e-5, "{:f}" },
		{ 0, "{:a}" },
		{ 1e-5, "{:e}" },
		{ 1e-5, "{:g}" },

		{ 1e-5, "{:.10f}" },
		{ 1e-5, "{:.10a}" },
		{ 1e-5, "{:.10e}" },
		{ 1e-5, "{:.10g}" },
};

#ifdef __CDT_PARSER__
#define $3 ""
#endif

m4_applyforeachdefine(((f),(d),(l),),
m4_syncline(1)«m4_dnl;

#ifndef _yIO_HAS_FLOAT$1
#error
#endif

#if _yIO_HAS_FLOAT$1

#define TYPE$1   _yIO_FLOAT$1
#define STRTO$1  _yIO_strto$1
#define PRI$1    _yIO_FLOAT_PRI$1

static inline
void test_onefloat_$1(const char *instr, TYPE$1 in,
		const char *format, double diffatmost) {
	errno = 0;
	Ychar *const format_native = yπformatf(Yc("{}"), format);
	_yIO_TEST_NOFAIL(errno == 0, "%d %s", errno, strerror(errno));
	_yIO_ASSERT(format_native);

	Ychar *str_native;
	errno = 0;
	int err = yπaprintf(&str_native, format_native, in);
	_yIO_TEST_NOFAIL(errno == 0 , "%s,%s %d %s", format, instr, errno, strerror(errno));
	free(format_native);
	if (strstr(instr, "_MAX") != NULL && (
			err == YIO_ERROR_ENOMEM ||
			err == YIO_ERROR_ENOSYS
			)) {
		// allow for failing here, printing LDLB_MAX is close to impossible....
		free(str_native);
		if (verbose) printf("%4s %1s%7s,%-15s OK_FAILURE %d %s\n",
					"π", "$1", format, instr, err, yio_strerror(err));
		return;
	}
	_yIO_ASSERT(str_native != NULL);

	char *const str = yformatf("{}", str_native);
	free(str_native);
	_yIO_ASSERT(str != NULL);

	char *endp;
	errno = 0;
	const TYPE$1 res = STRTO$1(str, &endp);
	const int errnostrto = errno;

	const TYPE$1 diff = _yIO_diff$1(in, res);

	if (verbose) printf("%4s %1s%7s,%-15s|%21s>%20"PRI$1"a %8"PRI$1"g<%g\n",
			"π", "$1", format, instr, str, res, diff, diffatmost);

	int failed = 0;
	if (res == _yIO_FLOAT_HUGE_VAL$1 || res == -_yIO_FLOAT_HUGE_VAL$1) {
		// this is actually fine, is chacked in diff
		failed |= _yIO_TEST_NOFAIL(errnostrto == 0, "overflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
	}
	if (res == 0) {
		failed |= _yIO_TEST(errnostrto == 0, "underflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
	}
	failed |= _yIO_TEST(diff <= diffatmost, "π %s,%s %"PRI$1"g<%g", format, instr, diff, diffatmost);

	if (!verbose && failed) printf("%4s %1s%7s,%-15s|%21s>%20"PRI$1"a %8"PRI$1"g<%g\n",
			"π", "$1", format, instr, str, res, diff, diffatmost);

	// _yIO_TEST_NOFAIL(fabs$1(in - res) < 0.05, "π %s,%s %20.30"PRI$1"g %20.30"PRI$1"g", format, instr, in, res);
	_yIO_TEST(endp == str + strlen(str));

	free(str);
}

static void test_floats_$1(void) {
	for(size_t i = 0; i < sizeof(_yIO_test_floatlist$1)/sizeof(*_yIO_test_floatlist$1); ++i) {
		const char *const instr = _yIO_test_floatlist$1[i].valstr;
		const TYPE$1 in = _yIO_test_floatlist$1[i].val;
		for(size_t j = 0; j < sizeof(formats)/sizeof(*formats); ++j) {
			const char * const format = formats[j].format;
			const double diffatmost = formats[j].diffatmost;
			assert(0 <= diffatmost);
			assert(diffatmost <= 1.0);
			test_onefloat_$1(instr, in, format, diffatmost);
		}
	}
}

#endif

») m4_syncline(1)m4_dnl;

int main() {
	// let newlib initialize
	errno = 0;
	setvbuf(stdout, NULL, _IOLBF, 0);

	test_floats_f();
	if (!_yIO_test_is_in_valgrind()) test_floats_l();
	test_floats_d();

	return 0;
}
