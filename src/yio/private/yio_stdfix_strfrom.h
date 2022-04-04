/**
 * @file
 * @date 2020-sie-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#if _yIO_HAS_STDFIX_TYPES

{% from 'yio/private/yio_stdfix.h' import j_STDFIX %}
{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
/**
 * Converts a $2 value to a string.
 * @param resultp @see _yIO_wur
 * @param lengthp @see _yIO_wur
 * @param precision0 Precision or negative when no precision.
 * @param spec0 The formatting specifier character or '\0' when missing.
 * @param val The value to convert.
 * @return 0 on success, negative on error.
 */
_yIO_wur _yIO_nn()
int _yIO_astrfrom$1(char **resultp, size_t *lengthp, int precision0, char spec0, $2 val);
{% endcall %}

#endif // _yIO_HAS_STDFIX_TYPES
