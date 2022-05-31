/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <assert.h>
#include <yio/yio_config.h>
#if YYIO_HAS_strfromf
#ifdef YIO_PRINT_FLOATS_WITH
#undef YIO_PRINT_FLOATS_WITH
#endif
#define YIO_PRINT_FLOATS_WITH  YIO_PRINT_FLOATS_WITH_STRFROM
#include "test_print_float_on_strto.c"
{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line
#ifndef YYΩIO_PRINT_FLOAT$1
#error YYΩIO_PRINT_FLOAT$1
#endif
#if defined __GNUC__ && ! defined __clang__ && ! defined __INTEL_COMPILER
static_assert(YYΩIO_PRINT_FLOAT$1 == YYΩIO_print_float_strfrom$1, "");
#endif
{% endcall %}
#line
#else
#include <yπio.h>
int main() {
	return yπprint("");
}
#endif
