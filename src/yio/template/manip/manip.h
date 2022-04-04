/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "../ctx.h"
#include "../../yio_config.h"
#include "slots.h"
#include "unhandled.h"
#include "print_bool.h"
#include "print_chars.h"
#include "print_int.h"
#include "print_float.h"
#include "print_wchars.h"
#include "print_uchars.h"
#include "print_time.h"
#include "print_stdfix.h"
#include "print_complex.h"
#include "print_modifiers.h"
#include "print_count.h"
#include "print_cfmt.h"

/**
 * @def _yΩIO_PRINT_FUNC_GENERIC
 * For one argument choose the printing function dynamically using _Generic macro
 */
#define _yΩIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS() \
				_yΩIO_PRINT_FUNC_GENERIC_WCHARS() \
				_yΩIO_PRINT_FUNC_GENERIC_UCHARS() \
				_yΩIO_PRINT_FUNC_GENERIC_BOOL() \
				_yΩIO_PRINT_FUNC_GENERIC_INTS() \
				_yΩIO_PRINT_FUNC_GENERIC_FLOATS() \
				_yΩIO_PRINT_GENERIC_TIME() \
				_yΩIO_PRINT_STDFIX() \
				_yΩIO_PRINT_COMPLEX() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS_SECOND_STAGE() \
				_yΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		default: _yΩIO_print_unhandled_type \
		)))
