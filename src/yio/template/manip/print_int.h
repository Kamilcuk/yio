/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

int _yΩIO_print_short(yπio_printctx_t *t);
int _yΩIO_print_ushort(yπio_printctx_t *t);
int _yΩIO_print_int(yπio_printctx_t *t);
int _yΩIO_print_uint(yπio_printctx_t *t);
int _yΩIO_print_long(yπio_printctx_t *t);
int _yΩIO_print_ulong(yπio_printctx_t *t);
int _yΩIO_print_llong(yπio_printctx_t *t);
int _yΩIO_print_ullong(yπio_printctx_t *t);

#ifndef _yIO_HAS_INT128
#error
#endif
#if _yIO_HAS_INT128
int _yΩIO_print___int128(yπio_printctx_t *t);
int _yΩIO_print_u__int128(yπio_printctx_t *t);
#define _yΩIO_PRINT_FUNC_GENERIC_INTS_INT128() \
		__int128: _yΩIO_print___int128, \
		unsigned __int128: _yΩIO_print_u__int128,
#else
#define _yΩIO_PRINT_FUNC_GENERIC_INTS_INT128()
#endif

#define _yΩIO_PRINT_FUNC_GENERIC_INTS() \
		short: _yΩIO_print_short, \
		unsigned short: _yΩIO_print_ushort, \
		int: _yΩIO_print_int, \
		unsigned int: _yΩIO_print_uint, \
		long: _yΩIO_print_long, \
		unsigned long: _yΩIO_print_ulong, \
		long long: _yΩIO_print_llong, \
		unsigned long long: _yΩIO_print_ullong, \
		_yΩIO_PRINT_FUNC_GENERIC_INTS_INT128()
