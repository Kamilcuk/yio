/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

int _yΩIO_print_wchar(yπio_printctx_t *t);
int _yΩIO_print_wcharpnt(yπio_printctx_t *t);
int _yΩIO_print_constwcharpnt(yπio_printctx_t *t);

/**
 * @defgroup ypw
 * @brief Wide characters printers.
 * @{
 */
/// Print wchar_t character.
#define yπpwchar(wchar)         yiocb(_yΩIO_print_wchar, (wchar))
/// Print a string of wchar_t characters.
#define yπpwstring(wstring)     yiocb(_yΩIO_print_wcharpnt, (wstring))
/**
 * @}
 */

#define _yΩIO_PRINT_FUNC_GENERIC_WCHARS() \
		wchar_t *: _yΩIO_print_wcharpnt, \
		const wchar_t *: _yΩIO_print_constwcharpnt,

#define _yΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		wchar_t: _yΩIO_print_wchar,
