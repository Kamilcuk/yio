/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

int _yIO_scan_int(yio_scanctx_t *t);
int _yIO_scan_short(yio_scanctx_t *t);
int _yIO_scan_ushort(yio_scanctx_t *t);
int _yIO_scan_int(yio_scanctx_t *t);
int _yIO_scan_uint(yio_scanctx_t *t);
int _yIO_scan_long(yio_scanctx_t *t);
int _yIO_scan_ulong(yio_scanctx_t *t);
int _yIO_scan_llong(yio_scanctx_t *t);
int _yIO_scan_ullong(yio_scanctx_t *t);
int _yIO_scan___int128(yio_scanctx_t *t);
int _yIO_scan_u__int128(yio_scanctx_t *t);


#ifdef _yIO_HAS_INT128
#define _yIO_SCAN_FUNC_GENERIc_INTS_128() \
		__int128 *: _yIO_scan___int128, \
		unsigned __int128 *: _yIO_scan_u__int128,
#else
#define _yIO_SCAN_FUNC_GENERIc_INTS_128()
#endif

#define _yIO_SCAN_FUNC_GENERIC_INTS() \
		short *: _yIO_scan_short, \
		unsigned short *: _yIO_scan_ushort, \
		int *: _yIO_scan_int, \
		unsigned int *: _yIO_scan_uint, \
		long *: _yIO_scan_long, \
		unsigned long *: _yIO_scan_ulong, \
		long long *: _yIO_scan_llong, \
		unsigned long long *: _yIO_scan_ullong, \
		_yIO_SCAN_FUNC_GENERIc_INTS_128()
