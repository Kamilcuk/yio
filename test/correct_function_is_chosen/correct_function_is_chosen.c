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
MASK(YYIO_print_float_printff)
MASK(YYIO_print_float_printfd)
MASK(YYIO_print_float_printfl)
MASK(YYIO_print_float_customf)
MASK(YYIO_print_float_customd)
MASK(YYIO_print_float_customl)

#define CHECK(FUNC) \
	do{ \
		printf("%s\n", gfunc); \
		YYIO_TEST(strcmp(gfunc, FUNC) == 0, "gfunc=%s", gfunc); \
	} while(0)

int main() {
#if YYIO_HAS_FLOATf
	yprintf("{}", 1.0f);
#if YYIO_HAS_strfromf
	CHECK("YYIO_print_float_strfromf");
#else
	CHECK("YYIO_print_float_customf");
	//CHECK("YYIO_print_float_printff");
#endif
#endif

#if YIO_HAS_FLOATd
	yprintf("{}", 1.0);
#if YYIO_HAS_strfromd
	CHECK("YYIO_print_float_strfromd");
#else
	CHECK("YYIO_print_float_customd");
	//CHECK("YYIO_print_float_printfd");
#endif
#endif

#if YIO_HAS_FLOATl
	yprintf("{}", 1.0l);
#if YYIO_HAS_strfroml
	CHECK("YYIO_print_float_strfroml");
#else
	CHECK("YYIO_print_float_customl");
	//CHECK("YYIO_print_float_printfl");
#endif
#endif
}
