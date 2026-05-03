/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

static const char *gfunc = "";

#define MASK(FUNC) \
	int FUNC(yio_printctx_t *t); \
	int __real_##FUNC(yio_printctx_t *t); \
	int __wrap_##FUNC(yio_printctx_t *t); \
	int __wrap_##FUNC(yio_printctx_t *t) { \
		gfunc = #FUNC; \
		return __real_##FUNC(t); \
	}

#ifndef YYIO_HAS_strfromf
#error YYIO_HAS_strfromf
#endif
#if YYIO_HAS_strfromf
MASK(YYIO_print_float_strfromf)
#endif
#ifndef YYIO_HAS_strfromd
#error YYIO_HAS_strfromd
#endif
#if YYIO_HAS_strfromd
MASK(YYIO_print_float_strfromd)
#endif
#ifndef YYIO_HAS_strfroml
#error YYIO_HAS_strfroml
#endif
#if YYIO_HAS_strfroml
MASK(YYIO_print_float_strfroml)
#endif
MASK(YYIO_print_float_ryuf)
MASK(YYIO_print_float_ryud)
MASK(YYIO_print_float_ryul)
MASK(YYIO_print_float_printff)
MASK(YYIO_print_float_printfd)
MASK(YYIO_print_float_printfl)
MASK(YYIO_print_float_customf)
MASK(YYIO_print_float_customd)
MASK(YYIO_print_float_customl)

#define CHECK(SUFF) \
	do{ \
		printf("called function is `%s`\n", gfunc); \
		const char *shouldbe; \
		if (YIO_FLOAT_BACKEND_STRFROM && YYIO_HAS_strfrom##SUFF) { \
			shouldbe = "YYIO_print_float_strfrom" #SUFF; \
		} else if (YIO_FLOAT_BACKEND_RYU && YYIO_has_float_ryu##SUFF) { \
			shouldbe = "YYIO_print_float_ryu" #SUFF; \
		} else { \
			shouldbe = "YYIO_print_float_custom" #SUFF; \
		} \
		YIO_TESTEXPR(strcmp(gfunc, shouldbe) == 0, \
							 "called function was gfunc=%s but it should be=%s", gfunc, shouldbe); \
		gfunc = ""; \
	} while(0)

int main() {
#if YIO_HAS_FLOATf
	yio_printf("{}", 1.0f);
	CHECK(f);
#endif

#if YIO_HAS_FLOATd
	yio_printf("{}", 1.0);
	CHECK(d);
#endif

#if YIO_HAS_FLOATl
	yio_printf("{}", 1.0l);
	CHECK(l);
#endif
}
