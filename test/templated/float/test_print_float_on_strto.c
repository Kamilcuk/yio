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
	long double diffatmost;
	const char *const format;
};

static const struct formats_s formats[] = {
		{ 1e-5, "{}" },
		{ 1e-5, "{:f}" },
		{ 1e-5, "{:F}" },
		{    0, "{:a}" },
		{    0, "{:A}" },
		{ 1e-5, "{:e}" },
		{ 1e-5, "{:E}" },
		{ 1e-5, "{:g}" },
		{ 1e-5, "{:G}" },
		{ 1e-9, "{:.10f}" },
		{ 1e-9, "{:.10a}" },
		{ 1e-9, "{:.10e}" },
		{ 1e-9, "{:.10g}" },
		{ 1e-20, "{:+#100.20a}" },
		{ 1e-40, "{:+#100.40a}" },
#if _yΩIO_PRINT_FLOATd != _yIO_print_float_customd
		{ 1e-20, "{:-#100.20f}" },
		{ 1e-20, "{:-#100.20e}" },
		{ 1e-20, "{:+#100.20g}" },
		{ 1e-40, "{:-#100.40f}" },
		{ 1e-40, "{:-#100.40e}" },
		{ 1e-40, "{:+#100.40g}" },
#endif
};

#ifdef __CDT_PARSER__
#define $3 ""
#endif

#define VERBOSEARGS(MODE, PRI) \
			"pi='%s'  mode=%s\n" \
			"\typrintf(\"%s\", %s)=%s\n" \
			"\tstrto(res)=%.40"PRI"f = %"PRI"a\n" \
			"\tdiff=%.10"PRI"g < %.10g\n", \
			"π", MODE, \
			format, instr, str, \
			res, res, \
			diff, diffatmost


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
_yIO_FLOAT$1 _yIO_diff$1(_yIO_FLOAT$1 in, _yIO_FLOAT$1 out) {
	//static _yIO_FLOAT$1 very_close = _yIO_FLOAT_C$1(1e-6);

	if (isnan(in)) {
		return isnan(out) ? 0 : 1;
	}
	if (isnan(out)) {
		return 1;
	}
	if (isinf(in)) {
		return isinf(in) != isinf(out);
	}
	if (isinf(out)) {
		// the conversion function resulted in INF from MAX value, it's fine
		if (_yIO_fabs$1(in) >= _yIO_FLOAT_MAX$1) return _yIO_FLOAT_EPSILON$1;
		return 1;
	}
	if (in == out) {
		return 0;
	}

	const _yIO_FLOAT$1 greater = in < out ? out : in;
	const _yIO_FLOAT$1 smaller = in < out ? in : out;
	// epsilong was converted to zero, (or zero to zero), that's fine
	if (_yIO_fabs$1(greater) <= _yIO_FLOAT_EPSILON$1) return _yIO_FLOAT_EPSILON$1;
	const _yIO_FLOAT$1 diff1 = (greater - smaller) / greater;
	assert(!isnan(diff1));
	assert(!isinf(diff1));
	const _yIO_FLOAT$1 diff = _yIO_fabs$1(diff1);
	assert(0 <= diff);
	assert(diff <= 1);
	return diff;
}

static inline
void test_onefloat_$1(const char *instr, TYPE$1 in,
		const char *format, double diffatmost) {
	errno = 0;
	Ychar *const format_native = yπformatf(Yc("{}"), format);
	_yIO_TEST_NOFAIL(errno == 0, "%s %d %s", format, errno, strerror(errno));
	_yIO_ASSERT(format_native);

	if (!instr) instr = "(null)";

	Ychar *str_native = NULL;
	errno = 0;
	int err = yπaprintf(&str_native, format_native, in);
	_yIO_TEST_NOFAIL(errno == 0, "%s,%s %d %s", format, instr, errno, strerror(errno));
	free(format_native);
	if (strstr(instr, "_MAX") != NULL && (
			err == YIO_ERROR_ENOMEM ||
			err == YIO_ERROR_ENOSYS
			)) {
		// allow for failing here, printing LDLB_MAX is close to impossible....
		free(str_native);
		// if (verbose)
		printf("%4s %1s%7s,%-15s OK_FAILURE %d %s\n",
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

	if (verbose) {
		printf(VERBOSEARGS("$1", PRI$1));
	}

	int failed = 0;
	if (errnostrto == ERANGE && (res == _yIO_FLOAT_HUGE_VAL$1 || res == -_yIO_FLOAT_HUGE_VAL$1)) {
		// Only allowed when converting the max values.
		if (!(_yIO_fabs$1(in) >= _yIO_FLOAT_MAX$1 && strstr(instr, "_MAX"))) {
			failed |= _yIO_TEST(errnostrto == 0, "overflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
		}
	} else if ((strstr(instr, "_MIN") || strstr(instr, "_EPSILON")) && strstr(format, "f")) {
	} else {
		failed |= _yIO_TEST(diff <= diffatmost, "π %s,%s %"PRI$1"g<%g", format, instr, diff, diffatmost);
	}
	if (res == 0) {
#ifdef __INTEL_COMPILER
		failed |= _yIO_TEST_NOFAIL(
#else
		failed |= _yIO_TEST(
#endif
			errnostrto == 0, "underflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
	}

	if (!verbose && failed) {
		printf(VERBOSEARGS("$1", PRI$1));
	}

	// _yIO_TEST_NOFAIL(fabs$1(in - res) < 0.05, "π %s,%s %20.30"PRI$1"g %20.30"PRI$1"g", format, instr, in, res);
	_yIO_TEST(endp == str + strlen(str));

	free(str);
}

static void test_floats_$1(void) {
	size_t start = 0;
#if defined(__INTEL_COMPILER) && _yΩIO_PRINT_FLOATd == _yIO_print_float_customd
	// icc bug: double d = 1e-307; d *= 0.1; assert(d == 1e-308); - fails
	start = _yIO_floatlist_exotics_cnt;
#endif
	for(size_t i = start; i < sizeof(_yIO_test_floatlist$1)/sizeof(*_yIO_test_floatlist$1); ++i) {
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
	// The first call suprisingly returns errno=2. Curiosly where.
	// Anyway, call it here, so it doesn't return errno=2 later.
	free(yformatf("{}", "{}"));

	test_floats_f();
	if (!_yIO_test_is_in_valgrind()) test_floats_l();
	test_floats_d();

	return 0;
}
