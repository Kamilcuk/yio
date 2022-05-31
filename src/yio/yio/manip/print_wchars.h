/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_WCHARS_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_WCHARS_H_
#include "../ctx.h"

#ifndef YIO_HAS_WCHAR_H
#error YIO_HAS_WCHAR_H
#endif
#if YIO_HAS_WCHAR_H
int YYΩIO_print_wchar(yπio_printctx_t *t);
int YYΩIO_print_constwcharpnt(yπio_printctx_t *t);

/**
 * @defgroup yπpw TMODEN yπpw
 * @ingroup yπio
 * @brief Wide characters printers.
 * @{
 */
/// Print wchar_t character.
#define yπpwchar(wchar)         yiocb(YYΩIO_print_wchar, (wchar_t)(wchar))
/// Print a string of wchar_t characters.
#define yπpwstring(wstring)     yiocb(YYΩIO_print_constwcharpnt, (const wchar_t*)(wstring))
/**
 * @}
 */

#define YYΩIO_PRINT_FUNC_GENERIC_WCHARS() \
		wchar_t *: YYΩIO_print_constwcharpnt, \
		const wchar_t *: YYΩIO_print_constwcharpnt,

#define YYΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		wchar_t: YYΩIO_print_wchar,
#else
#define YYΩIO_PRINT_FUNC_GENERIC_WCHARS()
#define YYΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE()
#endif
#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_WCHARS_H_ */
