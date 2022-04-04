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

int _yΩIO_print_right(yπio_printctx_t *t);
int _yΩIO_print_noright(yπio_printctx_t *t);
int _yΩIO_print_left(yπio_printctx_t *t);
int _yΩIO_print_center(yπio_printctx_t *t);
int _yΩIO_print_fixed(yπio_printctx_t *t);
int _yΩIO_print_scientific(yπio_printctx_t *t);
int _yΩIO_print_hexfloat(yπio_printctx_t *t);
int _yΩIO_print_persistent(yπio_printctx_t *t);
int _yΩIO_print_nopersistent(yπio_printctx_t *t);
int _yΩIO_print_precision(yπio_printctx_t *t);
int _yΩIO_print_width(yπio_printctx_t *t);
int _yΩIO_print_unitbuf(yπio_printctx_t *t);
int _yΩIO_print_nounitbuf(yπio_printctx_t *t);

/// Align output to the right.
#define yπpright()         yiocb(_yΩIO_print_right)
/// Align output to the left.
#define yπpleft()          yiocb(_yΩIO_print_left)
/// Center the output within width.
#define yπpcenter()        yiocb(_yΩIO_print_center)
/// Outputs the next float in fixed format.
#define yπpfixed()         yiocb(_yΩIO_print_fixed)
/// Outputs the next float in scientific format.
#define yπpscientific()    yiocb(_yΩIO_print_scientific)
/// Print floats using hexadecimal notation.
#define yπphexfloat()      yiocb(_yΩIO_print_hexfloat)
/// Set formatting precision.
#define yπpprecision(arg)  yiocb(_yΩIO_print_precision, _yIO_CAST_TO_INT(arg))
/// Set field width.
#define yπpwidth(arg)      yiocb(_yΩIO_print_width, _yIO_CAST_TO_INT(arg))
/// Print the unit.
#define yπpunitbuf()       yiocb(_yΩIO_print_unitbuf)
/// Do not print the unit.
#define yπpnounitbuf()     yiocb(_yΩIO_print_nounitbuf)

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
