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

#ifndef _yIO_HAS_strfromf
#error _yIO_HAS_strfromf
#endif
#if _yIO_HAS_strfromf
MASK(_yIO_print_float_strfromf)
#endif
#ifndef _yIO_HAS_strfromd
#error _yIO_HAS_strfromd
#endif
#if _yIO_HAS_strfromd
MASK(_yIO_print_float_strfromd)
#endif
#ifndef _yIO_HAS_strfroml
#error _yIO_HAS_strfroml
#endif
#if _yIO_HAS_strfroml
MASK(_yIO_print_float_strfroml)
#endif
MASK(_yIO_print_float_printff)
MASK(_yIO_print_float_printfd)
MASK(_yIO_print_float_printfl)
MASK(_yIO_print_float_stupidf)
MASK(_yIO_print_float_stupidd)
MASK(_yIO_print_float_stupidl)

#define CHECK(FUNC) \
	do{ \
		printf("%s\n", gfunc); \
		_yIO_TEST(strcmp(gfunc, FUNC) == 0, "gfunc=%s", gfunc); \
	} while(0)

int main() {
#if _yIO_HAS_FLOATf
	yprintf("{}", 1.0f);
#if __NEWLIB__
	CHECK("_yIO_print_float_stupidf");
#else
	CHECK("_yIO_print_float_strfromf");
	//CHECK("_yIO_print_float_printff");
#endif
#endif

#if _yIO_HAS_FLOATd
	yprintf("{}", 1.0);
#if __NEWLIB__
	CHECK("_yIO_print_float_stupidd");
#else
	CHECK("_yIO_print_float_strfromd");
	//CHECK("_yIO_print_float_printfd");
#endif
#endif

#if _yIO_HAS_FLOATl
	yprintf("{}", 1.0l);
#if __NEWLIB__
	CHECK("_yIO_print_float_stupidl");
#else
	CHECK("_yIO_print_float_strfroml");
	//CHECK("_yIO_print_float_printfl");
#endif
#endif
}
