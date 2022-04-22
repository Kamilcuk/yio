/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_MANIP_PRINT_WCHARS_H_
#define _yIO_YIO_YΩIO_MANIP_PRINT_WCHARS_H_

#ifndef _yIO_HAS_WCHAR_H
#error _yIO_HAS_WCHAR_H
#endif
#if _yIO_HAS_WCHAR_H
int _yΩIO_print_wchar(yπio_printctx_t *t);
int _yΩIO_print_constwcharpnt(yπio_printctx_t *t);

/**
 * @defgroup ypw
 * @brief Wide characters printers.
 * @{
 */
/// Print wchar_t character.
#define yπpwchar(wchar)         yiocb(_yΩIO_print_wchar, (wchar_t)(wchar))
/// Print a string of wchar_t characters.
#define yπpwstring(wstring)     yiocb(_yΩIO_print_constwcharpnt, (const wchar_t*)(wstring))
/**
 * @}
 */

#define _yΩIO_PRINT_FUNC_GENERIC_WCHARS() \
		wchar_t *: _yΩIO_print_constwcharpnt, \
		const wchar_t *: _yΩIO_print_constwcharpnt,

#define _yΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		wchar_t: _yΩIO_print_wchar,
#else
#define _yΩIO_PRINT_FUNC_GENERIC_WCHARS()
#define _yΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE()
#endif
#endif /* _yIO_YIO_YΩIO_MANIP_PRINT_WCHARS_H_ */
