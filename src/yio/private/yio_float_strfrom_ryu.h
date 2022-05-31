/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
#include "../yio_config.h"
#ifdef YYIO_PRIVATE
#include "yio_res.h"
#endif

{% call j_FOREACHAPPLY(["f", "d", "l"]) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

#ifdef YYIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param res
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YYIO_float_astrfrom_ryu$1(YYIO_res *res, int precision, char spec0, YYIO_FLOAT$1 val);
#endif

#define YYIO_has_float_ryu$1  1
#else
#define YYIO_has_float_ryu$1  0
#endif // YYIO_FLOAT$1
{% endcall %}

{% call j_FOREACHAPPLY([
		"f16", "f32", "f64", "f128",
		"f32x", "f64x", "f128x",
		"d32", "d64", "d128" ]) %}
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1
#ifndef YYIO_has_float_ryu$1
#define YYIO_has_float_ryu$1  0
#endif
#endif
{% endcall %}
#endif /* YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_ */
