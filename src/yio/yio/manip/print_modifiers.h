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

int _yIO_print_right(yio_printctx_t *t);
int _yIO_print_noright(yio_printctx_t *t);
int _yIO_print_left(yio_printctx_t *t);
int _yIO_print_center(yio_printctx_t *t);
int _yIO_print_fixed(yio_printctx_t *t);
int _yIO_print_scientific(yio_printctx_t *t);
int _yIO_print_hexfloat(yio_printctx_t *t);
int _yIO_print_persistent(yio_printctx_t *t);
int _yIO_print_nopersistent(yio_printctx_t *t);
int _yIO_print_precision(yio_printctx_t *t);
int _yIO_print_width(yio_printctx_t *t);
int _yIO_print_unitbuf(yio_printctx_t *t);
int _yIO_print_nounitbuf(yio_printctx_t *t);

/// Align output to the right.
#define ypright()         yiocb(_yIO_print_right)
/// Align output to the left.
#define ypleft()          yiocb(_yIO_print_left)
/// Center the output within width.
#define ypcenter()        yiocb(_yIO_print_center)
/// Outputs the next float in fixed format.
#define ypfixed()         yiocb(_yIO_print_fixed)
/// Outputs the next float in scientific format.
#define ypscientific()    yiocb(_yIO_print_scientific)
/// Print floats using hexadecimal notation.
#define yphexfloat()      yiocb(_yIO_print_hexfloat)
/// Set formatting precision.
#define ypprecision(arg)  yiocb(_yIO_print_precision, _yIO_CAST_TO_INT(arg))
/// Set field width.
#define ypwidth(arg)      yiocb(_yIO_print_width, _yIO_CAST_TO_INT(arg))
/// Print the unit.
#define ypunitbuf()       yiocb(_yIO_print_unitbuf)
/// Do not print the unit.
#define ypnounitbuf()     yiocb(_yIO_print_nounitbuf)

int _yIO_print_pfmt(yio_printctx_t *t);

/**
 * Format output by reading python format string.
 * @def yppfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 */
#define _yIO_yppfmt_0(fmt)               fmt
#define _yIO_yppfmt_1(fmt, spec)         fmt, _Generic((spec),int:(spec))
#define _yIO_yppfmt_2(fmt, spec, spec2)  fmt, _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yIO_yppfmt_N(_0,_1,_2,N,...)  _yIO_yppfmt_##N
#define yppfmt(...)  \
		yiocb(_yIO_print_pfmt, _yIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * @}
 */
