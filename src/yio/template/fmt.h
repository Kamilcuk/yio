/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include <stdbool.h>
{% if MODEX == 2 %}
#include <wchar.h>
{% elif MODEX == 3 %}
#include <uchar.h>
{% endif %}

static const Ychar YΩIO_NUL = Yc('\0');

static const Ychar YΩIO_ALIGN_LEFT = Yc('<');
static const Ychar YΩIO_ALIGN_RIGHT = Yc('>');
static const Ychar YΩIO_ALIGN_PADSIGN = Yc('=');
static const Ychar YΩIO_ALIGN_CENTER = Yc('^');

static const Ychar YΩIO_SIGN_ALWAYS = Yc('+');
static const Ychar YΩIO_SIGN_NEGATIVE = Yc('-');
static const Ychar YΩIO_SIGN_ALWAYSSPACE = Yc(' ');

static const Ychar YΩIO_GROUPING_COMMA = Yc(',');
static const Ychar YΩIO_GROUPING_UNDERSCORE = Yc('_');
static const Ychar YΩIO_GROUPING_NONE = Yc('\0');

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
	Ychar fill;
	/**
	 * May be one of '<' '>' '^' '=' characters or 0 when unset.
	 */
	Ychar align;
	/**
	 * May be on of '+' '-' ' ' character or 0 when unset.
	 */
	Ychar sign;
	/**
	 * May be set to '_' ',' or 0 when unset.
	 */
	Ychar grouping;
	/**
	 * The printing type character. The standard valid characters are "bcdeEfFgGnosxX%".
	 */
	Ychar type;
	/// The conversion specifier.
	Ychar conversion;
	/**
	 * Use of alternate form.
	 */
	bool hash;
};
/**
 * @}
 */

