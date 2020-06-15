/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config();

int _yIO_print_char(yio_printctx_t *t);
int _yIO_print_schar(yio_printctx_t *t);
int _yIO_print_uchar(yio_printctx_t *t);
int _yIO_print_charpnt(yio_printctx_t *t);
int _yIO_print_constcharpnt(yio_printctx_t *t);

#define _yIO_PRINT_FUNC_GENERIC_CHARS() \
		char: _yIO_print_char, \
		char*: _yIO_print_charpnt, \
		const char*: _yIO_print_constcharpnt,

#define _yIO_PRINT_FUNC_GENERIC_CHARS_SECOND_STAGE() \
		signed char: _yIO_print_schar, \
		unsigned char: _yIO_print_uchar,
