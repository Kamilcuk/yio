/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "../yio_config.h"
#include <stddef.h>

m4_applysync(«(
			(f), (d), (l),
			(d32), (d64), (d128),
)», «

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
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

#define _yIO_has_float_printf$1  1
#else
#define _yIO_has_float_printf$1  0
#endif

»)

m4_applysync(«(
		(f16), (f32), (f64), (f128),
		(f32x), (f64x), (f128x),
		(d32x), (d64x), (d128x),
)»,«
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1
#define _yIO_has_float_printf$1 0
#endif
»)


