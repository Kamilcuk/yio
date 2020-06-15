/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config();

int _yIO_print_wchar(yio_printctx_t *t);
int _yIO_print_wcharpnt(yio_printctx_t *t);
int _yIO_print_constwcharpnt(yio_printctx_t *t);

/**
 * @defgroup yp__ Printing overloads.
 * @{
 */
/// Print wchar_t character.
#define ypwchar(wchar)         yiocb(_yIO_print_wchar, wchar)
/// Print a string of wchar_t characters.
#define ypwstrint(wstring)     yiocb(_Generic(0+(wstring), \
		wchar_t *: _yIO_print_wstring, \
		const wchar_t *: _yIO_print_constwcharpnt), wstring)
/**
 * @}
 */

#define _yIO_PRINT_FUNC_GENERIC_WCHARS() \
		wchar_t *: _yIO_print_wcharpnt, \
		const wchar_t *: _yIO_print_constwcharpnt,

#define _yIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		wchar_t: _yIO_print_wchar,
