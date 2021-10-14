/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "yio_float.h"
#include "yio_decimal.h"
#include <stddef.h>

m4_applyforeachdefine(«(
			(f), (d), (l),
			(d32), (d64), (d128),
)», m4_syncline(1)«

#ifndef _yIO_HAS_FLOAT$1
#error
#endif

m4_ifelse(m4_regexp($1, «d[0-9][0-9]»), -1, «
#if _yIO_HAS_FLOAT$1
»,«
#if 0
»)»m4_syncline(1)«

/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param resultp @see _yIO_vec
 * @param lengthp @see _yIO_vec
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int _yIO_float_astrfrom_stupid$1(char **resultp, size_t *lengthp,
		int precision, char spec0, _yIO_FLOAT$1 val);

#define _yIO_has_float_stupid$1  1
#else
#define _yIO_has_float_stupid$1  0
#endif // _yIO_HAS_FLOAT$1

»)
