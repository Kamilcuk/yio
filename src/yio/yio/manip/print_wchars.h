/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

int _yIO_print_wchar(yio_printctx_t *t);
int _yIO_print_wcharpnt(yio_printctx_t *t);
int _yIO_print_constwcharpnt(yio_printctx_t *t);

/**
 * @defgroup ypw
 * @brief Wide characters printers.
 * @{
 */
/// Print wchar_t character.
#define ypwchar(wchar)         yiocb(_yIO_print_wchar, (wchar))
/// Print a string of wchar_t characters.
#define ypwstring(wstring)     yiocb(_yIO_print_wcharpnt, (wstring))
/**
 * @}
 */

#define _yIO_PRINT_FUNC_GENERIC_WCHARS() \
		wchar_t *: _yIO_print_wcharpnt, \
		const wchar_t *: _yIO_print_constwcharpnt,

#define _yIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		wchar_t: _yIO_print_wchar,
