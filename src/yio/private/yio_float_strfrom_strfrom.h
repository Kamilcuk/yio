/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_STRFROM_H_
#define _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_STRFROM_H_
#include "../yio_config.h"
#ifdef _yIO_PRIVATE
#include "yio_res.h"
#endif

{% call j_FOREACHAPPLY(j_FLOATS) %}

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

#ifndef _yIO_HAS_strfrom$1
#error
#endif

#ifdef _yIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using @c strfrom$1 function.
 * @param v
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int _yIO_float_astrfrom_strfrom$1(_yIO_res *v, int precision, char spec, _yIO_FLOAT$1 val);
#endif

#endif

// strfrom is always available
// in case of problems, users will get link errors, so they can implement
// strfrom themselves.
#define _yIO_has_float_strfrom$1  1

{% endcall %}
#endif /* _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_STRFROM_H_ */
