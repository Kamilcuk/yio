/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

#define YIO_PRIVATE 1
#include <yio/private/yio_float.h>
#include <yio/private/yio_float_strfrom_naive.h>
#include <yio/private/yio_float_strfrom_strfrom.h>
#include <yio/private/yio_float_strfrom_printf.h>
#include <yio/private/yio_float_strfrom_ryu.h>
#include <string.h>
#include <stdio.h>

static const char *gfunc = "";

#define DEFINE_WRAP(BACKEND, REPR) \
    int __wrap_YIO_float_astrfrom_##BACKEND##_##REPR(YIO_string *v, int precision, char spec, YIO_FLOAT_RP_##REPR val); \
    int __wrap_YIO_float_astrfrom_##BACKEND##_##REPR(YIO_string *v, int precision, char spec, YIO_FLOAT_RP_##REPR val) { \
        (void)v; (void)precision; (void)spec; (void)val; \
        gfunc = "YIO_float_astrfrom_" #BACKEND "_" #REPR; \
        return 0; \
    }

#define DEFINE_WRAPS(REPR) \
    DEFINE_WRAP(strfrom, REPR) \
    DEFINE_WRAP(ryu, REPR) \
    DEFINE_WRAP(printf, REPR) \
    DEFINE_WRAP(naive, REPR)

#ifdef YIO_FLOAT_RP_B16
DEFINE_WRAPS(B16)
#endif
#ifdef YIO_FLOAT_RP_B32
DEFINE_WRAPS(B32)
#endif
#ifdef YIO_FLOAT_RP_B64
DEFINE_WRAPS(B64)
#endif
#ifdef YIO_FLOAT_RP_B80
DEFINE_WRAPS(B80)
#endif
#ifdef YIO_FLOAT_RP_B128
DEFINE_WRAPS(B128)
#endif
#ifdef YIO_FLOAT_RP_D32
DEFINE_WRAPS(D32)
#endif
#ifdef YIO_FLOAT_RP_D64
DEFINE_WRAPS(D64)
#endif
#ifdef YIO_FLOAT_RP_D128
DEFINE_WRAPS(D128)
#endif

#define GET_SHOULDBE(REPR) \
	( YIO_has_float_astrfrom_strfrom_##REPR ? "YIO_float_astrfrom_strfrom_" #REPR : \
	  YIO_has_float_astrfrom_ryu_##REPR      ? "YIO_float_astrfrom_ryu_" #REPR : \
	  YIO_has_float_astrfrom_printf_##REPR   ? "YIO_float_astrfrom_printf_" #REPR : \
	  "YIO_float_astrfrom_naive_" #REPR )

#define CHECK(SUFF, REPR) \
    do{ \
        printf("called function is `%s` for SUFF=" #SUFF " REPR=" #REPR "\n", gfunc); \
        const char *shouldbe = GET_SHOULDBE(REPR); \
        YIO_TESTEXPR(strcmp(gfunc, shouldbe) == 0, \
                             "called function was gfunc=%s but it should be=%s", gfunc, shouldbe); \
        gfunc = ""; \
    } while(0)


int main() {
#if YIO_HAS_FLOATf
	yio_printf("{}", 1.0f);
#if YIO_RP_OF_f_IS_B32
	CHECK(f, B32);
#endif
#endif

#if YIO_HAS_FLOATd
	yio_printf("{}", 1.0);
#if YIO_RP_OF_d_IS_B64
	CHECK(d, B64);
#endif
#endif

#if YIO_HAS_FLOATl
	yio_printf("{}", 1.0l);
#if YIO_RP_OF_l_IS_B80
	CHECK(l, B80);
#elif YIO_RP_OF_l_IS_B128
	CHECK(l, B128);
#elif YIO_RP_OF_l_IS_B64
	CHECK(l, B64);
#endif
#endif
    return 0;
}
