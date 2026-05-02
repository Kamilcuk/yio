/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio/yio_config.h>
#if YYIO_HAS_strfromf
#ifdef YIO_FLOAT_BACKEND
#undef YIO_FLOAT_BACKEND
#endif
#undef YIO_FLOAT_BACKEND
#define YIO_FLOAT_BACKEND  STRFROM
#include <assert.h>
#include "test_print_float_on_strto.c"
{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line
#ifndef YYIO_PRINT_FLOAT$1
#error YYIO_PRINT_FLOAT$1
#endif
#if defined __GNUC__ && ! defined __clang__ && ! defined __INTEL_COMPILER
#endif
{% endcall %}
#line
#else
#include <yio.h>
int main() {
	return yio_stream("");
}
#endif
