/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_float.h"
#include <stddef.h>

m4_applyforeachdefine(«((f), (d), (l))», «m4_dnl;
#ifndef _yIO_HAS_FLOAT$1
#error
#endif
#if _yIO_HAS_FLOAT$1
/**
 * Convert the floating number val according to specified precision
 * and specification using snprintf function.
 * @param resultp @see _yIO_vec
 * @param lengthp @see _yIO_vec
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int _yIO_float_astrfrom_printf$1(char **resultp, size_t *lengthp,
		int precision, char spec, _yIO_FLOAT$1 val);
#endif
») m4_dnl;

