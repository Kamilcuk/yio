/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_FMT_H_
#define _yIO_YIO_YΩIO_FMT_H_
#include <stdbool.h>
{% if MODEX == 2 %}
#include <wchar.h>
{% elif MODEX == 3 %}
#include <uchar.h>
{% endif %}

static const TCHAR YΩIO_NUL = TC('\0');

static const TCHAR YΩIO_ALIGN_LEFT = TC('<');
static const TCHAR YΩIO_ALIGN_RIGHT = TC('>');
static const TCHAR YΩIO_ALIGN_PADSIGN = TC('=');
static const TCHAR YΩIO_ALIGN_CENTER = TC('^');

static const TCHAR YΩIO_SIGN_ALWAYS = TC('+');
static const TCHAR YΩIO_SIGN_NEGATIVE = TC('-');
static const TCHAR YΩIO_SIGN_ALWAYSSPACE = TC(' ');

static const TCHAR YΩIO_GROUPING_COMMA = TC(',');
static const TCHAR YΩIO_GROUPING_UNDERSCORE = TC('_');
static const TCHAR YΩIO_GROUPING_NONE = TC('\0');

static const bool YΩIO_HASH_ON = true;
static const bool YΩIO_HASH_OFF = false;

/**
 * @defgroup printfmt Printing format
 * @{
 */
/**
 * Printing formatting options.
 */
struct yπio_printfmt_s {
	/**
	 * The field width. Negative when unset.
	 */
	int width;
	/**
	 * The field precision. Negative when unset.
	 */
	int precision;
	/**
	 * Filling character.
	 */
	TCHAR fill;
	/**
	 * May be one of '<' '>' '^' '=' characters or 0 when unset.
	 */
	TCHAR align;
	/**
	 * May be on of '+' '-' ' ' character or 0 when unset.
	 */
	TCHAR sign;
	/**
	 * May be set to '_' ',' or 0 when unset.
	 */
	TCHAR grouping;
	/**
	 * The printing type character. The standard valid characters are "bcdeEfFgGnosxX%".
	 */
	TCHAR type;
	/// The conversion specifier.
	TCHAR conversion;
	/**
	 * Use of alternate form.
	 */
	bool hash;
};
/**
 * @}
 */

#endif /* _yIO_YIO_YΩIO_FMT_H_ */
