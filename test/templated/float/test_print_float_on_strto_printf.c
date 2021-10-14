/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_syncline(1)m4_dnl;
#include <assert.h>
#include <yio/yio_config.h>
#ifdef YIO_PRINT_FLOATS_WITH
#undef YIO_PRINT_FLOATS_WITH
#endif
#define YIO_PRINT_FLOATS_WITH  YIO_PRINT_FLOATS_WITH_PRINTF
#include "test_print_float_on_strto.c"
m4_applyforeachdefine(((f),(d),(l)), m4_syncline(1)«m4_dnl;
#ifndef _yΩIO_PRINT_FLOAT$1
#error _yΩIO_PRINT_FLOAT$1
#endif
#if defined __GNUC__ && ! defined __clang__ && ! defined __INTEL_COMPILER
static_assert(_yΩIO_PRINT_FLOAT$1 == _yΩIO_print_float_printf$1, "");
#endif
»)

