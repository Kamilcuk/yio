/**
 * @file
 * @date 2020-sie-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_
#define YYIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#ifndef YYIO_HAS_STDFIX_TYPES
#error YYIO_HAS_STDFIX_TYPES
#endif
#if YYIO_HAS_STDFIX_TYPES

#ifdef YYIO_PRIVATE
#include "yio_string.h"
{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef YYIO_STDFIX_$3
/**
 * Converts a $2 value to a string.
 * @param o YYIO_string object for result
 * @param pf
 * @param val The value to convert.
 * @return 0 on success, negative on error.
 */
YYIO_wur YYIO_nn()
int YYIO_strfrom$1(YYIO_string *o, const struct yio_printfmt_s *pf, $2 val);
#endif

{% endcall %}
#endif

#endif // YYIO_HAS_STDFIX_TYPES

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_
