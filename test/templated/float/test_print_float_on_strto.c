/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test_private.h>
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
#if YYΩIO_PRINT_FLOATd != YYIO_print_float_customd
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


{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error
#endif

#if YIO_HAS_FLOAT$1

#define TYPE$1   YYIO_FLOAT$1
#define STRTO$1  YYIO_strto$1
#define PRI$1    YYIO_FLOAT_PRI$1

static inline
YYIO_FLOAT$1 YYIO_diff$1(YYIO_FLOAT$1 in, YYIO_FLOAT$1 out) {
	//static YYIO_FLOAT$1 very_close = YYIO_FLOAT_C$1(1e-6);

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
		if (YYIO_fabs$1(in) >= YYIO_FLOAT_MAX$1) return YYIO_FLOAT_EPSILON$1;
		return 1;
	}
	if (in == out) {
		return 0;
	}

	const YYIO_FLOAT$1 greater = in < out ? out : in;
	const YYIO_FLOAT$1 smaller = in < out ? in : out;
	// epsilong was converted to zero, (or zero to zero), that's fine
	if (YYIO_fabs$1(greater) <= YYIO_FLOAT_EPSILON$1) return YYIO_FLOAT_EPSILON$1;
	const YYIO_FLOAT$1 diff1 = (greater - smaller) / greater;
	assert(!isnan(diff1));
	assert(!isinf(diff1));
	const YYIO_FLOAT$1 diff = YYIO_fabs$1(diff1);
	assert(0 <= diff);
	assert(diff <= 1);
	return diff;
}

static inline
void test_onefloat_$1(const char *instr, TYPE$1 in,
		const char *format, double diffatmost) {
	errno = 0;
	TCHAR *const format_native = yπformatf(TC("{}"), format);
	YIO_TESTEXPR_NOFAIL(errno == 0, "%s %d %s", format, errno, strerror(errno));
	YYIO_ASSERT(format_native);

	if (!instr) instr = "(null)";

	TCHAR *str_native = NULL;
	errno = 0;
	int err = yπaprintf(&str_native, format_native, in);
	YIO_TESTEXPR_NOFAIL(errno == 0, "%s,%s %d %s", format, instr, errno, strerror(errno));
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
	YYIO_ASSERT(str_native != NULL);

	char *const str = yformatf("{}", str_native);
	free(str_native);
	YYIO_ASSERT(str != NULL);

	char *endp;
	errno = 0;
	const TYPE$1 res = STRTO$1(str, &endp);
	const int errnostrto = errno;

	const TYPE$1 diff = YYIO_diff$1(in, res);

	if (verbose) {
		printf(VERBOSEARGS("$1", PRI$1));
	}

	int failed = 0;
	if (errnostrto == ERANGE && (res == YYIO_FLOAT_HUGE_VAL$1 || res == -YYIO_FLOAT_HUGE_VAL$1)) {
		// Only allowed when converting the max values.
		if (!(YYIO_fabs$1(in) >= YYIO_FLOAT_MAX$1 && strstr(instr, "_MAX"))) {
			failed |= YIO_TESTEXPR(errnostrto == 0, "overflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
		}
	} else if ((strstr(instr, "_MIN") || strstr(instr, "_EPSILON")) && strstr(format, "f")) {
	} else {
		failed |= YIO_TESTEXPR(diff <= diffatmost, "π %s,%s %"PRI$1"g<%g", format, instr, diff, diffatmost);
	}
	if (res == 0) {
#ifdef __INTEL_COMPILER
		failed |= YIO_TESTEXPR_NOFAIL(
#else
		failed |= YIO_TESTEXPR(
#endif
			errnostrto == 0, "underflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
	}

	if (!verbose && failed) {
		printf(VERBOSEARGS("$1", PRI$1));
	}

	// YIO_TESTEXPR_NOFAIL(fabs$1(in - res) < 0.05, "π %s,%s %20.30"PRI$1"g %20.30"PRI$1"g", format, instr, in, res);
	YIO_TESTEXPR(endp == str + strlen(str));

	free(str);
}

static void test_floats_$1(void) {
	size_t start = 0;
#if defined(__INTEL_COMPILER) && YYΩIO_PRINT_FLOATd == YYIO_print_float_customd
	// icc bug: double d = 1e-307; d *= 0.1; assert(d == 1e-308); - fails
	start = YYIO_floatlist_exotics_cnt;
#endif
	for(size_t i = start; i < sizeof(YYIO_test_floatlist$1)/sizeof(*YYIO_test_floatlist$1); ++i) {
		const char *const instr = YYIO_test_floatlist$1[i].valstr;
		const TYPE$1 in = YYIO_test_floatlist$1[i].val;
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

{% endcall %}
#line

int main() {
	// let newlib initialize
	errno = 0;
	setvbuf(stdout, NULL, _IOLBF, 0);
	// The first call suprisingly returns errno=2. Curiosly where.
	// Anyway, call it here, so it doesn't return errno=2 later.
	free(yformatf("{}", "{}"));

	test_floats_f();
	if (!YYIO_test_is_in_valgrind()) test_floats_l();
	test_floats_d();

	return 0;
}
