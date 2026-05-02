/**
 * @file
 * @date 2026-05-01
 * @author Gemini CLI
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio/yio_config.h>
#undef YIO_FLOAT_BACKEND
#define YIO_FLOAT_BACKEND  RYU
#include <yio_test_private.h>
#include <yio.h>
#include "test_print_float_on_strto.c"
