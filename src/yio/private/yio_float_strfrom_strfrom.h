/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "../yio_config.h"
#include <stddef.h>

m4_applysync(«(
			(f), (d), (l),
			(f16), (f32), (f64), (f128),
			(f32x), (f64x), (f128x),
			(d32), (d64), (d128),
)», «

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

#ifndef _yIO_HAS_strfrom$1
#error
#endif

/**
 * Convert the floating number val according to specified precision
 * and specification using @c strfrom$1 function.
 * @param resultp @see _yIO_vec
 * @param lengthp @see _yIO_vec
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int _yIO_float_astrfrom_strfrom$1(char **resultp, size_t *lengthp,
		int precision, char spec, _yIO_FLOAT$1 val);

#endif

// strfrom is always available
// in case of problems, users will get link errors, so they can implement
// strfrom themselves.
#define _yIO_has_float_strfrom$1  1

»);
