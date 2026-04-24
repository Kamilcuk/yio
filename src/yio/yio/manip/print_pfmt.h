/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_PFMT_H_
#define YYIO_YIO_YIO_MANIP_PRINT_PFMT_H_
#include "../ctx.h"

/**
 * @defgroup yio_pfmt yio_pfmt
 * @ingroup yio
 * @brief Python printing manipulator.
 * @{
 **/

int YYIO_print_pfmt(yio_printctx_t *t);

/**
 * @def yio_pfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 * @brief Format output by reading python format string.
 */
#define YYIO_yppfmt_0(fmt)               ((void)_Generic((fmt),const char*:1,char*:(void)1),(fmt))
#define YYIO_yppfmt_1(fmt, spec)         YYIO_yppfmt_0(fmt), _Generic((spec),int:(spec))
#define YYIO_yppfmt_2(fmt, spec, spec2)  YYIO_yppfmt_0(fmt), _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define YYIO_yppfmt_N(_0,_1,_2,N,...)  YYIO_yppfmt_##N
#define yio_pfmt(...)  \
		yio_callback(YYIO_print_pfmt, YYIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * @}
 */
#endif /* YYIO_YIO_YIO_MANIP_PRINT_PFMT_H_ */
