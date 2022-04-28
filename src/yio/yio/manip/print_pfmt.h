/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_MANIP_PRINT_PFMT_H_
#define _yIO_YIO_YΩIO_MANIP_PRINT_PFMT_H_

/**
 * @defgroup yπppfmt TMODEN yπppfmt
 * @ingroup yπio
 * @brief Python printing manipulator.
 * @{
 **/

int _yΩIO_print_pfmt(yπio_printctx_t *t);

/**
 * @def yppfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 * @brief Format output by reading python format string.
 */
#define _yΩIO_yppfmt_0(fmt)               ((void)_Generic((fmt),const TCHAR*:1,TCHAR*:(void)1),(fmt))
#define _yΩIO_yppfmt_1(fmt, spec)         _yΩIO_yppfmt_0(fmt), _Generic((spec),int:(spec))
#define _yΩIO_yppfmt_2(fmt, spec, spec2)  _yΩIO_yppfmt_0(fmt), _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yΩIO_yppfmt_N(_0,_1,_2,N,...)  _yΩIO_yppfmt_##N
#define yπppfmt(...)  \
		yiocb(_yΩIO_print_pfmt, _yΩIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * @}
 */
#endif /* _yIO_YIO_YΩIO_MANIP_PRINT_PFMT_H_ */