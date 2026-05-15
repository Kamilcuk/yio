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
		{ 1e-20, "{:-#100.20f}" },
		{ 1e-20, "{:-#100.20e}" },
		{ 1e-20, "{:+#100.20g}" },
		{ 1e-40, "{:-#100.40f}" },
		{ 1e-40, "{:-#100.40e}" },
		{ 1e-40, "{:+#100.40g}" },
};

#define VERBOSEARGS(PRI) \
			"pi='%s'\n" \
			"\typrintf(\"%s\", %s)=%s\n" \
			"\tstrto(res)=%.40"PRI"f = %"PRI"a\n" \
			"\tdiff=%.10"PRI"g < %.10g\n", \
			"", \
			format, instr, str, \
			res, res, \
			diff, diffatmost


{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error YIO_HAS_FLOAT$1 is not defined
#endif

#if YIO_HAS_FLOAT$1

#define TYPE$1   YIO_FLOAT$1
#define STRTO$1  YIO_strto$1
#define PRI$1    YIO_FLOAT_PRI$1

static inline
YIO_FLOAT$1 YIO_diff$1(YIO_FLOAT$1 in, YIO_FLOAT$1 out) {
	//static YIO_FLOAT$1 very_close = YIO_FLOAT_C$1(1e-6);

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
		if (YIO_fabs$1(in) >= YIO_FLOAT_MAX$1) return YIO_FLOAT_EPSILON$1;
		return 1;
	}
	if (in == out) {
		return 0;
	}

	const YIO_FLOAT$1 greater = in < out ? out : in;
	const YIO_FLOAT$1 smaller = in < out ? in : out;
	// epsilong was converted to zero, (or zero to zero), that's fine
	if (YIO_fabs$1(greater) <= YIO_FLOAT_EPSILON$1) return YIO_FLOAT_EPSILON$1;
	const YIO_FLOAT$1 diff1 = (greater - smaller) / greater;
	assert(!isnan(diff1));
	assert(!isinf(diff1));
	const YIO_FLOAT$1 diff = YIO_fabs$1(diff1);
	assert(0 <= diff);
	assert(diff <= 1);
	return diff;
}

static inline
void test_onefloat_$1(const char *instr, TYPE$1 in,
		const char *format, double diffatmost) {
	if (!instr) instr = "(null)";

	static char str[10000];
	errno = 0;
	int err = yio_snprint(str, sizeof(str), format, in);

	if (strstr(instr, "_MAX") != NULL && (
			err == YIO_ERROR_ENOMEM ||
			err == YIO_ERROR_ENOSYS
			)) {
		// allow for failing here, printing LDLB_MAX is close to impossible....
		// if (verbose)
		printf("%4s %1s%7s,%-15s OK_FAILURE %d %s\n",
					"", "$1", format, instr, err, yio_strerror(err));
		return;
	}
	if (err < 0) {
		printf("yio_snprint failed: err=%d %s, errno=%d %s, in=%g, format=%s, instr=%s\n",
				err, yio_strerror(err), errno, strerror(errno), (double)in, format, instr);
	}
	YIO_TESTEXPR_ASSERT(err >= 0);

	char *endp;
	errno = 0;
	const TYPE$1 res = STRTO$1(str, &endp);
	const int errnostrto = errno;

	const TYPE$1 diff = YIO_diff$1(in, res);

	if (verbose) {
		printf(VERBOSEARGS(PRI$1));
	}

	int failed = 0;
	if (errnostrto == ERANGE && (res == YIO_FLOAT_HUGE_VAL$1 || res == -YIO_FLOAT_HUGE_VAL$1)) {
		// Only allowed when converting the max values.
		if (!(YIO_fabs$1(in) >= YIO_FLOAT_MAX$1 && strstr(instr, "_MAX"))) {
			failed |= YIO_TESTEXPR_ASSERT(errnostrto == 0, "overflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
		}
	} else if ((strstr(instr, "_MIN") || strstr(instr, "_EPSILON")) && strstr(format, "f")) {
	} else {
#if defined(__arm__)
		failed |= YIO_TESTEXPR_NOFAIL(diff <= diffatmost, " %s,%s %"PRI$1"g<%g", format, instr, diff, diffatmost);
#else
		failed |= YIO_TESTEXPR_ASSERT(diff <= diffatmost, " %s,%s %"PRI$1"g<%g", format, instr, diff, diffatmost);
#endif
	}
	if (res == 0) {
#ifdef __INTEL_COMPILER
		failed |= YIO_TESTEXPR_NOFAIL(
#else
		failed |= YIO_TESTEXPR_ASSERT(
#endif
			errnostrto == 0, "underflow %s,%s %d %s", format, instr, errnostrto, strerror(errnostrto));
	}

	if (!verbose && failed) {
		printf(VERBOSEARGS(PRI$1));
	}

	// YIO_TESTEXPR_NOFAIL(fabs$1(in - res) < 0.05, " %s,%s %20.30"PRI$1"g %20.30"PRI$1"g", format, instr, in, res);
	YIO_TESTEXPR_ASSERT(endp == str + strlen(str));
}

static void test_floats_$1(void) {
	size_t start = 0;
#if defined(__INTEL_COMPILER) && YIO_PRINT_FLOATd == YIO_print_float_customd
	// icc bug: double d = 1e-307; d *= 0.1; assert(d == 1e-308); - fails
	start = YIO_floatlist_exotics_cnt;
#endif
	for(size_t i = start; i < sizeof(YIO_test_floatlist$1)/sizeof(*YIO_test_floatlist$1); ++i) {
		const char *const instr = YIO_test_floatlist$1[i].valstr;
		const TYPE$1 in = YIO_test_floatlist$1[i].val;
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
#if YIO_ENABLE_MALLOC
	char *tmp_fmt = NULL;
	yio_asprint(&tmp_fmt, "{}", "{}");
	free(tmp_fmt);
#endif

	test_floats_f();
	if (!YIO_test_is_in_valgrind()) test_floats_l();
	test_floats_d();

	return 0;
}
