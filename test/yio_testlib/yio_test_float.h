/**
 * @file
 * @date 2020-08-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio/private/yio_float.h>
#include <yio/private/yio_float_strfrom_printf.h>
#include <yio/private/yio_float_strfrom_strfrom.h>
#include <yio/private/yio_float_strfrom_stupid.h>
#include <float.h>
#include <math.h>
#include <assert.h>

m4_applyforeachdefine(`(
		(f, ,  FLT, F),
		(d, ,  DBL,  ),
		(l, , LDBL, L),
)~, m4_syncline(1)`m4_dnl;

#ifndef _yIO_HAS_FLOAT$1
#error
#endif

#if _yIO_HAS_FLOAT$1

struct _yIO_test_floatlist$1_s {
	const char *valstr;
	_yIO_FLOAT$1 val;
};

static const struct _yIO_test_floatlist$1_s _yIO_test_floatlist$1[] = {

		// start from 12 to remove them
#define _yIO_floatlist_exotics_cnt  12
#define M(x)  { #x, x }, { "-"#x, -x }
		M(INFINITY),
		M(NAN),
		M(HUGE_VAL$4),
		M($3_MIN),
		M($3_EPSILON),
		M($3_MAX),
#undef M

#define M(x) { #x, _yIO_FLOAT_C$1(x) }, { "-"#x, _yIO_FLOAT_C$1(-x) }
		M(123.456),
		M(0.4583577231),
		M(0.4895080385),
		M(0.7159709543),
		M(0.2711019543),
		M(0.3258227735),
		M(0.0400117192),
		M(0.2934020244),
		M(0.1484394331),
		M(0.9406671988),
		M(0.2367093763),
		M(1186215999.4476),
		M(463615916.78411),
		M(836946053.61027),
		M(271227406.8737),
		M(75253610.964957),
		M(1788428586.1672),
		M(1958800950.0879),
		M(1099820471.4879),
		M(838851634.60938),
		M(1582063030.2633),
		M(7797.8845744696),
		M(15231.437642725),
		M(5729.548728091),
		M(18914.650627363),
		M(964.6160971069),
		M(67.2082175679),
		M(15814.56973058),
		M(16050.524290657),
		M(15651.24164915),
		M(8115.8194332141),
		M(0.0),
		M(0.1),
		M(0.2),
		M(0.3),
		M(0.4),
		M(0.5),
		M(0.6),
		M(0.7),
		M(0.8),
		M(0.9),
		M(1.0),
		M(2.0),
		M(3.0),
		M(4.0),
		M(5.0),
		M(6.0),
		M(7.0),
		M(8.0),
		M(9.0),
		M(0.01),
		M(0.02),
		M(0.03),
		M(0.04),
		M(0.05),
		M(0.06),
		M(0.07),
		M(0.08),
		M(0.09),
		M(1111111111.99),
		M(9234567890.123),
		M(9234569999.99),
		M(0.9123456789),
		M(123.456),
		M(1111111111.99),
		M(9234567890.123),
		M(9234569999.99),
		M(0e-200),
		M(0.9123456789),
		M(0x1.ffep-2),
		M(0x1.fffffffffffffep-2),
		M(1.49),
		M(2.49),
		M(3.49),
		M(4.49),
		M(5.49),
		M(6.49),
		M(7.49),
		M(8.49),
		M(9.49),
		M(1.51),
		M(2.51),
		M(3.51),
		M(4.51),
		M(5.51),
		M(6.51),
		M(7.51),
		M(8.51),
		M(9.51),
		M(10.0),
		M(9.5),
		M(9.555555),
		M(1.9999998807907104),
		M(1.0/3.0),
#undef M

};

struct _yIO_astrfroms$1_s {
	int (*astrfrom)(char **resultp, size_t *lengthp, int precision, char type, _yIO_FLOAT$1 val);
	const char *astrfrom_str;
};

static const struct _yIO_astrfroms$1_s  _yIO_astrfroms$1[] = {
#define T(a)  { a, #a, }
#ifdef _yIO_HAVE_strfrom$1
		T(_yIO_float_astrfrom_strfrom$1),
#endif
		T(_yIO_float_astrfrom_stupid$1),
		T(_yIO_float_astrfrom_printf$1)
#undef T
};

static inline
_yIO_FLOAT$1 _yIO_diff$1(_yIO_FLOAT$1 in, _yIO_FLOAT$1 out) {
	static _yIO_FLOAT$1 very_close = _yIO_FLOAT_C$1(1e-6);

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
		if (_yIO_fabs$1(in) >= _yIO_FLOAT_MAX$1) return very_close;
		return 1;
	}
	if (in == out) {
		return 0;
	}

	const _yIO_FLOAT$1 greater = in < out ? out : in;
	const _yIO_FLOAT$1 smaller = in < out ? in : out;
	// epsilong was converted to zero, (or zero to zero), that's fine
	if (_yIO_fabs$1(greater) <= _yIO_FLOAT_EPSILON$1) return very_close;
	const _yIO_FLOAT$1 diff1 = (greater - smaller) / greater;
	assert(!isnan(diff1));
	assert(!isinf(diff1));
	const _yIO_FLOAT$1 diff = _yIO_fabs$1(diff1);
	assert(0 <= diff);
	assert(diff <= 1);
	return diff;
}

#endif

~)
