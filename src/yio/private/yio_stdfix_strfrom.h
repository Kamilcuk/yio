/**
 * @file
 * @date 2020-sie-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_
#define YIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#ifndef YIO_HAS_STDFIX_TYPES
#error YIO_HAS_STDFIX_TYPES
#endif
#if YIO_HAS_STDFIX_TYPES

#ifdef YIO_PRIVATE
#include "yio_buf.h"
{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef YIO_STDFIX_$3
/**
 * Converts a $2 value to a string.
 * @param o YIO_buf object for result
 * @param pf
 * @param val The value to convert.
 * @return 0 on success, negative on error.
 */
YIO_wur YIO_nn()
int YIO_strfrom$1(YIO_buf *o, const struct yio_printfmt_s *pf, $2 val);
#endif

{% endcall %}
#endif

#endif // YIO_HAS_STDFIX_TYPES

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_
