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
	int __wrap_##FUNC(yio_printctx_t *t) { \
		gfunc = #FUNC; \
		return __real_##FUNC(t); \
	}

#ifdef _yIO_HAS_strfromf
MASK(_yIO_print_float_strfromf)
#endif
#ifdef _yIO_HAS_strfrom
MASK(_yIO_print_float_strfrom)
#endif
#ifdef _yIO_HAS_strfroml
MASK(_yIO_print_float_strfroml)
#endif
MASK(_yIO_print_float_printff)
MASK(_yIO_print_float_printf)
MASK(_yIO_print_float_printfl)
MASK(_yIO_print_float_stupidf)
MASK(_yIO_print_float_stupid)
MASK(_yIO_print_float_stupidl)

#define CHECK(FUNC) \
	do{ \
		printf("%s\n", gfunc); \
		_yIO_TEST(strcmp(gfunc, FUNC) == 0, "gfunc=%s", gfunc); \
	} while(0)

int main() {
	yprintf("{}", 1.0f);
#ifdef _yIO_HAS_strfromf
	CHECK("_yIO_print_float_strfromf");
#elif defined YIO_FLOATS_PREFER_CUSTOM
	CHECK("_yIO_print_float_stupidf");
#else
	CHECK("_yIO_print_float_printff");
#endif

	yprintf("{}", 1.0);
#ifdef _yIO_HAS_strfrom
	CHECK("_yIO_print_float_strfrom");
#elif defined YIO_FLOATS_PREFER_CUSTOM
	CHECK("_yIO_print_float_stupid");
#else
	CHECK("_yIO_print_float_printf");
#endif

	yprintf("{}", 1.0l);
#ifdef _yIO_HAS_strfroml
	CHECK("_yIO_print_float_strfroml");
#elif defined YIO_FLOATS_PREFER_CUSTOM
	CHECK("_yIO_print_float_stupidl");
#else
	CHECK("_yIO_print_float_printfl");
#endif
}
