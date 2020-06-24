/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config(yio);

int _yIO_scan_floatf(yio_scanctx_t *t);
int _yIO_scan_floatpnt(yio_scanctx_t *t);
int _yIO_scan_float(yio_scanctx_t *t);
int _yIO_scan_floatl(yio_scanctx_t *t);

#define _yIO_SCAN_FUNC_GENERIC_FLOATS() \
		float: _yIO_scan_floatf, \
		float *: _yIO_scan_floatpnt, \
		double: _yIO_scan_float, \
		long double: _yIO_scan_floatl,
