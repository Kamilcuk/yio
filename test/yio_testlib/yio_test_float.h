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
#include <yio/private/yio_float_strfrom_custom.h>
#include <float.h>
#include <math.h>
#include <assert.h>

#ifdef __INTEL_COMPILER
#pragma warning disable 1188
#endif

{% call j_FOREACHAPPLY([
		["f", "",  "FLT", "F"],
		["d", "",  "DBL",  ""],
		["l", "", "LDBL", "L"],
	]) %}
#line

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
		M(0.0),
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
		M(289643189.86512),
		M(2016712781.0609),
		M(1699010479.2088),
		M(228915587.8934),
		M(730705637.65974),
		M(1939533384.0968),
		M(1303683268.3929),
		M(1430875890.3337),
		M(1738724809.1903),
		M(1399713878.3482),
		M(1.0),
		M(2.0),
		M(3.0),
		M(4.0),
		M(5.0),
		M(6.0),
		M(7.0),
		M(8.0),
		M(9.0),
		M(0.1),
		M(0.2),
		M(0.3),
		M(0.4),
		M(0.5),
		M(0.6),
		M(0.7),
		M(0.8),
		M(0.9),
		M(0.01),
		M(0.02),
		M(0.03),
		M(0.04),
		M(0.05),
		M(0.06),
		M(0.07),
		M(0.08),
		M(0.09),
		M(0.001),
		M(0.002),
		M(0.003),
		M(0.004),
		M(0.005),
		M(0.006),
		M(0.007),
		M(0.008),
		M(0.009),
		M(0.0001),
		M(0.0002),
		M(0.0003),
		M(0.0004),
		M(0.0005),
		M(0.0006),
		M(0.0007),
		M(0.0008),
		M(0.0009),
		M(0.00001),
		M(0.00002),
		M(0.00003),
		M(0.00004),
		M(0.00005),
		M(0.00006),
		M(0.00007),
		M(0.00008),
		M(0.00009),
		M(0.000001),
		M(0.000002),
		M(0.000003),
		M(0.000004),
		M(0.000005),
		M(0.000006),
		M(0.000007),
		M(0.000008),
		M(0.000009),
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
		T(_yIO_float_astrfrom_strfrom$1),
		T(_yIO_float_astrfrom_custom$1),
		T(_yIO_float_astrfrom_printf$1)
#undef T
};

#endif

{% endcall %}
