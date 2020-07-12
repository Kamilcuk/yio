/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config(yio);
#pragma once
#include <stdbool.h>

static const Ychar YIO_NUL = Yc('\0');

static const Ychar YIO_ALIGN_LEFT = Yc('<');
static const Ychar YIO_ALIGN_RIGHT = Yc('>');
static const Ychar YIO_ALIGN_PADSIGN = Yc('=');
static const Ychar YIO_ALIGN_CENTER = Yc('^');

static const Ychar YIO_SIGN_ALWAYS = Yc('+');
static const Ychar YIO_SIGN_NEGATIVE = Yc('-');
static const Ychar YIO_SIGN_ALWAYSSPACE = Yc(' ');

static const Ychar YIO_GROUPING_COMMA = Yc(',');
static const Ychar YIO_GROUPING_UNDERSCORE = Yc('_');
static const Ychar YIO_GROUPING_NONE = Yc('\0');

static const bool YIO_HASH_ON = true;
static const bool YIO_HASH_OFF = false;

/**
 * @defgroup printfmt Printing format
 * @{
 */
/**
 * Printing formatting options.
 */
struct yio_printfmt_s {
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
	/**
	 * Use of alternate form.
	 */
	bool hash;
};
/**
 * @}
 */

/**
 * @defgroup scanfmt Yio scanning format
 * @{
 */
/**
 * Scanning formatting options.
 */
struct yio_scanfmt_s {
	/**
	 * Field width - how many character can you read from input.
	 * Negative means endless.
	 */
	int width;
	/**
	 * TODO: pointer to "[^abc]" or "[def]"
	 */
	const Ychar *set;
	/**
	 * 0 or '*', when input should be ignored.
	 */
	Ychar ignore;
	/**
	 * Scanning type, standard valid characters are "iduoxfegacspn".
	 */
	Ychar type;
};
/**
 * @}
 */

