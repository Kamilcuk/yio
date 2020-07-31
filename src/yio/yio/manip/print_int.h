/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

int _yIO_print_short(yio_printctx_t *t);
int _yIO_print_ushort(yio_printctx_t *t);
int _yIO_print_int(yio_printctx_t *t);
int _yIO_print_uint(yio_printctx_t *t);
int _yIO_print_long(yio_printctx_t *t);
int _yIO_print_ulong(yio_printctx_t *t);
int _yIO_print_llong(yio_printctx_t *t);
int _yIO_print_ullong(yio_printctx_t *t);

#ifdef _yIO_HAS_INT128
int _yIO_print___int128(yio_printctx_t *t);
int _yIO_print_u__int128(yio_printctx_t *t);
#define _yIO_PRINT_FUNC_GENERIC_INTS_INT128() \
		__int128: _yIO_print___int128, \
		unsigned __int128: _yIO_print_u__int128,
#else
#define _yIO_PRINT_FUNC_GENERIC_INTS_INT128()
#endif

#define _yIO_PRINT_FUNC_GENERIC_INTS() \
		short: _yIO_print_short, \
		unsigned short: _yIO_print_ushort, \
		int: _yIO_print_int, \
		unsigned int: _yIO_print_uint, \
		long: _yIO_print_long, \
		unsigned long: _yIO_print_ulong, \
		long long: _yIO_print_llong, \
		unsigned long long: _yIO_print_ullong, \
		_yIO_PRINT_FUNC_GENERIC_INTS_INT128()
