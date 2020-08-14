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

struct formats_s {
	float percent_equal;
	const char *const format;
};

static const struct formats_s formats[] = {
		{ 0.9999, "{}" },
		{ 0.9999, "{:f}" },
		{ 1, "{:a}" },
		{ 0.9999, "{:e}" },
		{ 0.9999, "{:g}" },
		{ 0.9999, "{:.10f}" },
};

#ifdef __CDT_PARSER__
#define $3 ""
#endif

m4_applyforeachdefine((
		(f),
		(d),
		(l),
), m4_syncline(1)`m4_dnl;

#ifndef _yIO_HAS_FLOAT$1
#error
#endif

#if _yIO_HAS_FLOAT$1

#define TYPE$1   _yIO_FLOAT$1
#define STRTO$1  _yIO_strto$1
#define PRI$1    _yIO_FLOAT_PRI$1

struct floatlist_$1_s {
	const char *valstr;
	TYPE$1 val;
};

static const struct floatlist_$1_s floatlist_$1[] = {
#define _(x) { #x, x }
		_(123.456),
		_(INFINITY),
		_(NAN),
		_(_yIO_HUGE_VAL$1),
		_(0.4583577231),
		_(0.4895080385),
		_(0.7159709543),
		_(0.2711019543),
		_(0.3258227735),
		_(0.0400117192),
		_(0.2934020244),
		_(0.1484394331),
		_(0.9406671988),
		_(0.2367093763),
		_(1186215999.4476),
		_(463615916.78411),
		_(836946053.61027),
		_(271227406.8737),
		_(75253610.964957),
		_(1788428586.1672),
		_(1958800950.0879),
		_(1099820471.4879),
		_(838851634.60938),
		_(1582063030.2633),
		_(7797.8845744696),
		_(15231.437642725),
		_(5729.548728091),
		_(18914.650627363),
		_(964.6160971069),
		_(67.2082175679),
		_(15814.56973058),
		_(16050.524290657),
		_(15651.24164915),
		_(8115.8194332141),
		_(0.0),
		_(0.1),
		_(0.2),
		_(0.3),
		_(0.4),
		_(0.5),
		_(0.6),
		_(0.7),
		_(0.8),
		_(0.9),
		_(1.0),
		_(2.0),
		_(3.0),
		_(4.0),
		_(5.0),
		_(6.0),
		_(7.0),
		_(8.0),
		_(9.0),
		_(0.01),
		_(0.02),
		_(0.03),
		_(0.04),
		_(0.05),
		_(0.06),
		_(0.07),
		_(0.08),
		_(0.09),
		_(1111111111.99),
		_(9234567890.123),
		_(9234569999.99),
		_(0.9123456789),
		_(123.456),
		_(1111111111.99),
		_(9234567890.123),
		_(9234569999.99),
		_(0e-200),
		_(0.9123456789),
		_(0x1.ffep-2),
		_(0x1.fffffffffffffep-2),
		_(1.49),
		_(2.49),
		_(3.49),
		_(4.49),
		_(5.49),
		_(6.49),
		_(7.49),
		_(8.49),
		_(9.49),
		_(1.51),
		_(2.51),
		_(3.51),
		_(4.51),
		_(5.51),
		_(6.51),
		_(7.51),
		_(8.51),
		_(9.51),
		_(10),
		_(9.5),
		_(9.555555),
#undef _
};

static inline
TYPE$1 _yIO_diff$1(TYPE$1 a, TYPE$1 b) {
	if (isnan(a)) {
		return isnan(b);
	}
	if (isinf(a)) {
		return isinf(a) == isinf(b);
	}
	if (a == 0) {
		return b == 0;
	}

	const TYPE$1 diff = a < b ? a / b : b / a;
	assert(0 <= diff);
	assert(diff <= 1);
	return diff;
}

static inline
void test_onefloat_$1(const char *instr, TYPE$1 in,
		const char *format, float percent_equal) {
	Ychar *const format_native = yπformatf(Yc("{}"), format);
	_yIO_TEST(format_native);

	Ychar *const str_native = yπformatf(format_native, in);
	_yIO_TEST(str_native != NULL);
	free(format_native);

	char *const str = yformatf("{}", str_native);
	_yIO_TEST(str != NULL);
	free(str_native);

	char *endp;
	errno = 0;
	const TYPE$1 res = STRTO$1(str, &endp);

	const TYPE$1 diff = _yIO_diff$1(in, res);
	Yprintf("%4s %1s%7s,%-15s->%21s->%-20.10"PRI$1"g %.6"PRI$1"f%%>=%.6f\n",
			"π", "$1", format, instr, str, res, diff, percent_equal);

	if (res == _yIO_HUGE_VAL$1 || res == -_yIO_HUGE_VAL$1) {
		_yIO_TEST(errno == 0, "%d", errno);
	}
	if(percent_equal == 1) {
		_yIO_TEST(in == res || (isnan(in) && isnan(res)), "π %s,%s %"PRI$1"f<%f\n", format, instr, diff, percent_equal);
	} else {
		_yIO_TEST(diff >= percent_equal, "π %s,%s %"PRI$1"f<%f\n", format, instr, diff, percent_equal);
	}
	// _yIO_TEST_NOFAIL(fabs$1(in - res) < 0.05, "π %s,%s %20.30"PRI$1"g %20.30"PRI$1"g", format, instr, in, res);
	_yIO_TEST(endp == str + strlen(str));

	free(str);
}

static void test_floats_$1(void) {
	for(size_t i = 0; i < sizeof(floatlist_$1)/sizeof(*floatlist_$1); ++i) {
		const char *const instr = floatlist_$1[i].valstr;
		const TYPE$1 in = floatlist_$1[i].val;
		for(size_t j = 0; j < sizeof(formats)/sizeof(*formats); ++j) {
			const char * const format = formats[j].format;
			const float percent_equal = formats[j].percent_equal;
			assert(0 <= percent_equal);
			assert(percent_equal <= 1.0);
			test_onefloat_$1(instr, in, format, percent_equal);
		}
	}
}

#endif

~)m4_syncline(1)m4_dnl;

int main() {
	// let newlib initialize
	errno = 0;
	setvbuf(stdout, NULL, _IOLBF, 0);

	test_floats_f();

	test_floats_d();
	if (!_yIO_test_is_in_valgrind()) {
		test_floats_l();
	}

	return 0;
}
