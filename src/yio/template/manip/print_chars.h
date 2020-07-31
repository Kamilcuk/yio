/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

int _yΩIO_print_char(yπio_printctx_t *t);
int _yΩIO_print_schar(yπio_printctx_t *t);
int _yΩIO_print_uchar(yπio_printctx_t *t);
int _yΩIO_print_charpnt(yπio_printctx_t *t);
int _yΩIO_print_constcharpnt(yπio_printctx_t *t);

#define _yΩIO_PRINT_FUNC_GENERIC_CHARS() \
		char: _yΩIO_print_char, \
		char*: _yΩIO_print_charpnt, \
		const char*: _yΩIO_print_constcharpnt,

#define _yΩIO_PRINT_FUNC_GENERIC_CHARS_SECOND_STAGE() \
		signed char: _yΩIO_print_schar, \
		unsigned char: _yΩIO_print_uchar,
