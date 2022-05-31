/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_STRFROM_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_STRFROM_H_
#include "../yio_config.h"
#ifdef YYIO_PRIVATE
#include "yio_res.h"
#endif

{% call j_FOREACHAPPLY(j_FLOATS) %}

#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

#ifndef YYIO_HAS_strfrom$1
#error
#endif

#ifdef YYIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using @c strfrom$1 function.
 * @param v
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YYIO_float_astrfrom_strfrom$1(YYIO_res *v, int precision, char spec, YYIO_FLOAT$1 val);
#endif

#endif

// strfrom is always available
// in case of problems, users will get link errors, so they can implement
// strfrom themselves.
#define YYIO_has_float_strfrom$1  1

{% endcall %}
#endif /* YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_STRFROM_H_ */
