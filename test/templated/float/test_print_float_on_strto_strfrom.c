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
#if _yIO_HAS_strfromf
#ifdef YIO_PRINT_FLOATS_WITH
#undef YIO_PRINT_FLOATS_WITH
#endif
#define YIO_PRINT_FLOATS_WITH  YIO_PRINT_FLOATS_WITH_STRFROM
#include "test_print_float_on_strto.c"
m4_applyforeachdefine(((f),(d),(l)), m4_syncline(1)`m4_dnl;
#ifndef _yΩIO_PRINT_FLOAT$1_PRINTING_FUNC
#error _yΩIO_PRINT_FLOAT$1_PRINTING_FUNC
#endif
static_assert(_yΩIO_PRINT_FLOAT$1_PRINTING_FUNC == _yΩIO_print_float_strfrom$1, "_yΩIO_PRINT_FLOAT$1_PRINTING_FUNC == _yΩIO_print_float_strfrom$1");
~)
#else
#include <yπio.h>
int main() {
	return yπprint("");
}
#endif
