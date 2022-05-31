/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_PFMT_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_PFMT_H_
#include "../ctx.h"

/**
 * @defgroup yπppfmt TMODEN yπppfmt
 * @ingroup yπio
 * @brief Python printing manipulator.
 * @{
 **/

int YYΩIO_print_pfmt(yπio_printctx_t *t);

/**
 * @def yppfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 * @brief Format output by reading python format string.
 */
#define YYΩIO_yppfmt_0(fmt)               ((void)_Generic((fmt),const TCHAR*:1,TCHAR*:(void)1),(fmt))
#define YYΩIO_yppfmt_1(fmt, spec)         YYΩIO_yppfmt_0(fmt), _Generic((spec),int:(spec))
#define YYΩIO_yppfmt_2(fmt, spec, spec2)  YYΩIO_yppfmt_0(fmt), _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define YYΩIO_yppfmt_N(_0,_1,_2,N,...)  YYΩIO_yppfmt_##N
#define yπppfmt(...)  \
		yiocb(YYΩIO_print_pfmt, YYΩIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * @}
 */
#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_PFMT_H_ */
