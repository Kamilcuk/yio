/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_PFMT_H_
#define YIO_YIO_YIO_MANIP_PRINT_PFMT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

/**
 * @defgroup yio_pfmt yio_pfmt
 * @ingroup yio
 * @brief Python printing manipulator.
 * @{
 **/

int YIO_print_pfmt(yio_printctx_t *t);

/**
 * @def yio_pfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 * @brief Format output by reading python format string.
 */
#define YIO_yppfmt_0(fmt)               (_Generic((fmt),const char*:(fmt),char*:(fmt)))
#define YIO_yppfmt_1(fmt, spec)         YIO_yppfmt_0(fmt), _Generic((spec),int:(spec))
#define YIO_yppfmt_2(fmt, spec, spec2)  YIO_yppfmt_0(fmt), _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define YIO_yppfmt_N(_0,_1,_2,N,...)  YIO_yppfmt_##N
#define yio_pfmt(...)  \
		yio_callback(YIO_print_pfmt, YIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_PFMT_H_
