/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_syncline(1)m4_dnl;
#define YIO_PRINT_FLOATS_WITH  YIO_PRINT_FLOATS_WITH_PRINTF
#include <assert.h>
#include <yio/yio_config.h>
#include "test_print_float_on_strto.c"

m4_applysync(((f),(d),(l)), «

#ifndef _yΩIO_PRINT_FLOAT$1
#error  _yΩIO_PRINT_FLOAT$1
#endif
#if defined __GNUC__ && ! defined __clang__ && ! defined __INTEL_COMPILER
static_assert(_yΩIO_PRINT_FLOAT$1 == _yΩIO_print_float_printf$1, "");
#endif

»)

