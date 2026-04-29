/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_WCHARS_H_
#define YYIO_YIO_YIO_MANIP_PRINT_WCHARS_H_
#include "../ctx.h"

#ifndef YIO_HAS_WCHAR_H
#error YIO_HAS_WCHAR_H
#endif
#if YIO_HAS_WCHAR_H
int YYIO_print_wchar(yio_printctx_t *t);
int YYIO_print_constwcharpnt(yio_printctx_t *t);

/**
 * @defgroup ypw ypw
 * @ingroup yio
 * @brief Wide characters printers.
 * @{
 */
/// Print wchar_t character.
#define yio_pwchar(wchar)         yio_callback(YYIO_print_wchar, (wchar_t)(wchar))
/// Print a string of wchar_t characters.
#define yio_pwstring(wstring)     yio_callback(YYIO_print_constwcharpnt, (const wchar_t*)(wstring))
/**
 * @}
 */


#define YYIO_PRINT_FUNC_GENERIC_WCHARS() \
		YYIO_OVERLOAD_TYPE_FUNC(wchar_t *, YYIO_print_constwcharpnt) \
		YYIO_OVERLOAD_TYPE_FUNC(const wchar_t *, YYIO_print_constwcharpnt)

#ifndef YYIO_WCHAR_T_IS_UNIQUE
#error YYIO_WCHAR_T_IS_UNIQUE is not defiend
#endif
#if YYIO_WCHAR_T_IS_UNIQUE
#define YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		YYIO_OVERLOAD_TYPE_FUNC(wchar_t, YYIO_print_wchar)
#else
#define YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE()
#endif

#else
#define YYIO_PRINT_FUNC_GENERIC_WCHARS()
#define YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE()
#endif
#endif /* YYIO_YIO_YIO_MANIP_PRINT_WCHARS_H_ */
