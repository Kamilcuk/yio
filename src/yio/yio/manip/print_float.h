/**
 * @file print_float.h
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config();
#include <yio/yio_public.h>

m4_applyforeachdefine(`(
		(, f),
		(, fpnt),
		(, ),
		(, l)
)~,`m4_dnl;
int _yIO_print_float$2(yio_printctx_t *t);
~)m4_dnl;

#define _yIO_PRINT_FUNC_GENERIC_FLOATS() \
		m4_dnl;
m4_applyforeachdefine(`(
		(float, f),
		(float *, fpnt),
		(double, ),
		(long double, l)
)~, `$1: _yIO_print_float$2,~)m4_dnl;

