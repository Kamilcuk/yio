/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

/**
 * @defgroup yp__mod
 * @brief Printing modifiers.
 * @{
 **/

int _yΩIO_print_pfmt(yπio_printctx_t *t);

/**
 * Format output by reading python format string.
 * @def yppfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 */
#define _yΩIO_yppfmt_0(fmt)               fmt
#define _yΩIO_yppfmt_1(fmt, spec)         fmt, _Generic((spec),int:(spec))
#define _yΩIO_yppfmt_2(fmt, spec, spec2)  fmt, _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yΩIO_yppfmt_N(_0,_1,_2,N,...)  _yΩIO_yppfmt_##N
#define yπppfmt(...)  \
		yiocb(_yΩIO_print_pfmt, _yΩIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))


int _yΩIO_print_width_size_t(yπio_printctx_t *t);

/**
 * @}
 */
