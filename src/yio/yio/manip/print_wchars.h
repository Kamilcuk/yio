/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_WCHARS_H_
#define YIO_YIO_YIO_MANIP_PRINT_WCHARS_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx.h"

#ifndef YIO_HAS_WCHAR_H
#error YIO_HAS_WCHAR_H
#endif
#if YIO_HAS_WCHAR_H
int YIO_print_wchar(yio_printctx_t *t);
int YIO_print_constwcharpnt(yio_printctx_t *t);

/**
 * @defgroup ypw ypw
 * @ingroup yio
 * @brief Wide characters printers.
 * @{
 */
/// Print wchar_t character.
#define yio_pwchar(wchar)         yio_callback(YIO_print_wchar, (wchar_t)(wchar))
/// Print a string of wchar_t characters.
#define yio_pwstring(wstring)     yio_callback(YIO_print_constwcharpnt, (const wchar_t*)(wstring))
/**
 * @}
 */


#define YIO_PRINT_FUNC_GENERIC_WCHARS(X, XALIAS) \
		YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, wchar_t, YIO_print_constwcharpnt)

#ifndef YIO_HAS_UNIQUE_WCHAR_T
#error YIO_HAS_UNIQUE_WCHAR_T is not defiend
#endif
#if YIO_HAS_UNIQUE_WCHAR_T
#define YIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE(X, XALIAS) \
		X(wchar_t, YIO_print_wchar)
#else
#define YIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE(X, XALIAS)
#endif

#else
#define YIO_PRINT_FUNC_GENERIC_WCHARS(X, XALIAS)
#define YIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE(X, XALIAS)
#endif


#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_WCHARS_H_
