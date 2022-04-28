/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
#define _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
#include "../yio_config.h"
#ifdef _yIO_PRIVATE
#include "yio_res.h"
#endif

{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

#ifdef _yIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param res
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int _yIO_float_astrfrom_ryu$1(_yIO_res *res, int precision, char spec0, _yIO_FLOAT$1 val);
#endif

#define _yIO_has_float_ryu$1  1
#else
#define _yIO_has_float_ryu$1  0
#endif // _yIO_FLOAT$1
{% endcall %}

{% call j_FOREACHAPPLY([
		"f16", "f32", "f64", "f128",
		"f32x", "f64x", "f128x",
		"d32", "d64", "d128" ]) %}
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1
#ifndef _yIO_has_float_ryu$1
#define _yIO_has_float_ryu$1  0
#endif
#endif
{% endcall %}
#endif /* _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_ */
