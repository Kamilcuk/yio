/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

#define YYIO_PRIVATE 1
#include <yio/private/yio_float.h>
#include <yio/private/yio_float_strfrom_naive.h>
#include <yio/private/yio_float_strfrom_ryu.h>
#include <yio/private/yio_float_strfrom_strfrom.h>
#include <yio/private/yio_float_strfrom_printf.h>

static const char *gfunc = "";

#define MASK(BACKEND, REPR) \
	int __wrap_YYIO_float_astrfrom_##BACKEND##_##REPR(YYIO_string *v, int precision, char spec, ...); \
	int __wrap_YYIO_float_astrfrom_##BACKEND##_##REPR(YYIO_string *v, int precision, char spec, ...) { \
		(void)v; (void)precision; (void)spec; \
		gfunc = "YYIO_float_astrfrom_" #BACKEND "_" #REPR; \
		return 0; \
	}

#ifdef YYIO_FLOAT_REPR_B32
MASK(strfrom, B32)
MASK(ryu, B32)
MASK(printf, B32)
MASK(naive, B32)
#endif

#ifdef YYIO_FLOAT_REPR_B64
MASK(strfrom, B64)
MASK(ryu, B64)
MASK(printf, B64)
MASK(naive, B64)
#endif

#ifdef YYIO_FLOAT_REPR_B80
MASK(strfrom, B80)
MASK(ryu, B80)
MASK(printf, B80)
MASK(naive, B80)
#endif

#ifdef YYIO_FLOAT_REPR_B128
MASK(strfrom, B128)
MASK(ryu, B128)
MASK(printf, B128)
MASK(naive, B128)
#endif

#define CHECK(SUFF, REPR) \
	do{ \
		printf("called function is `%s` for SUFF=" #SUFF " REPR=" #REPR "\n", gfunc); \
		const char *shouldbe; \
		if (YIO_FLOAT_BACKEND_STRFROM && YYIO_has_float_strfrom_##REPR) { \
			shouldbe = "YYIO_float_astrfrom_strfrom_" #REPR; \
		} else if (YIO_FLOAT_BACKEND_RYU && YYIO_has_float_ryu_##REPR) { \
			shouldbe = "YYIO_float_astrfrom_ryu_" #REPR; \
		} else if (YIO_FLOAT_BACKEND_PRINTF && YYIO_has_float_printf_##REPR) { \
			shouldbe = "YYIO_float_astrfrom_printf_" #REPR; \
		} else { \
			shouldbe = "YYIO_float_astrfrom_naive_" #REPR; \
		} \
		YIO_TESTEXPR(strcmp(gfunc, shouldbe) == 0, \
							 "called function was gfunc=%s but it should be=%s", gfunc, shouldbe); \
		gfunc = ""; \
	} while(0)

int main() {
#if YIO_HAS_FLOATf
	yio_printf("{}", 1.0f);
#if YYIO_REPR_OF_f_IS_B32
	CHECK(f, B32);
#elif YYIO_REPR_OF_f_IS_B64
	CHECK(f, B64);
#endif
#endif

#if YIO_HAS_FLOATd
	yio_printf("{}", 1.0);
#if YYIO_REPR_OF_d_IS_B64
	CHECK(d, B64);
#endif
#endif

#if YIO_HAS_FLOATl
	yio_printf("{}", 1.0l);
#if YYIO_REPR_OF_l_IS_B80
	CHECK(l, B80);
#elif YYIO_REPR_OF_l_IS_B128
	CHECK(l, B128);
#elif YYIO_REPR_OF_l_IS_B64
	CHECK(l, B64);
#endif
#endif
}
