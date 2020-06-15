/**
 * @file scan_modifiers.h
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config();

int _yIO_scan_ignore(yio_scanctx_t *t);
int _yIO_scan_width_int(yio_scanctx_t *t) ;
int _yIO_scan_width_size_t(yio_scanctx_t *t);

/**
 * @defgroup ys_mod
 * @brief Scanning modifiers.
 * @{
 */
/// Ignore next input.
#define ysignore()         yiocb(_yIO_scan_ignore)
/// Set the maximum field width. Negative disables it.
#define yswidth(width)  \
		yiocb(_Generic((width), \
				int: _yIO_scan_width_int, \
				size_t: _yIO_scan_width_size_t \
		), width)
/**
 * @}
 */
