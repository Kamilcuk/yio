/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
#define _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
#include "../yio_config.h"
#include <stddef.h>

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#line
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
{% if V.0 in ["f", "d", "l", "d32", "d64", "d128"] %}
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
{% else %}
#define _yIO_has_float_printf$1 0
{% endif %}
{% endcall %}

#endif /* _yIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_ */
