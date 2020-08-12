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

struct formats_s {
	int digits_precision;
	const char *const format;
};

static const struct formats_s formats[] = {
		{ 5, "{}" },
		{ 5, "{:f}" },
		{ 5, "{:a}" },
		{ 5, "{:e}" },
		{ 5, "{:g}" },
		{ 10, "{:.10f}" },
};

#ifdef __CDT_PARSER__
#define $3 ""
#endif

m4_applyforeachdefine((
		(f,       float,  "",  f),
		( ,      double, "l",  d),
		(l, long double, "L", ld),
), m4_syncline(1)`m4_dnl;

typedef $2 TYPE$1;
#define PRI$1 ""$3""
#define strtofloat$1 strto$4

static const char TYPEstr$1[] = "$4";

struct floatlist_$1_s {
	const char *valstr;
	TYPE$1 val;
};

static const struct floatlist_$1_s floatlist_$1[] = {
#define _(x) { #x, x }
		_(123.456),
//		_(INFINITY),
		_(NAN),
//		_(HUGE_VAL$3),
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
#undef _
};

static inline long extract_digits$1(TYPE$1 v, int digits) {
	if (isnan(v)) return 0;
	if (isinf(v)) return 0;
	long long exp = 0;
	TYPE$1 t = v;
	while (t > 1) {
		t /= 10;
		exp++;
	}
	while (t < 0.1) {
		t *= 10;
		exp--;
	}
	const long ret = v / pow$1(10, exp - digits);
	return ret;
}

static void test_onefloat_$1(const char *instr, const TYPE$1 in, const char *format) {
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
	const TYPE$1 res = strtofloat$1(str, &endp);

	const int digits = 6;
	const long indigits = extract_digits$1(in, digits);
	const long resdigits = extract_digits$1(res, digits);
	Yprintf("%4s %2s %8s %20.10"PRI$1"g %21s %20.10"PRI$1"g %6ld %6ld\n", "π", TYPEstr$1, format, in, str, res, indigits, resdigits);

	_yIO_TEST(errno == 0, "%d", errno);
	_yIO_TEST(indigits - resdigits <= 1 , "π %s,%s %ld %ld\n", format, instr, indigits, resdigits);
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
			test_onefloat_$1(instr, in, format);
		}
	}
}

~)m4_syncline(1)m4_dnl;

int main() {
	test_floats_f();
	test_floats_();
	if (!_yIO_test_is_in_valgrind()) {
		test_floats_l();
	}
	return 0;
}
