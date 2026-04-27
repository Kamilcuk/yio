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
#include "../yio_config.h"
#ifndef YYIO_HAS_STDFIX_TYPES
#error YYIO_HAS_STDFIX_TYPES
#endif
#if YYIO_HAS_STDFIX_TYPES

#ifdef YYIO_PRIVATE
#include "yio_string.h"
{% from 'yio/private/yio_stdfix.h' import j_STDFIX %}
{% call j_FOREACHAPPLY(j_STDFIX) %}
#line

/**
 * Converts a $2 value to a string.
 * @param o YYIO_string object for result
 * @param pf
 * @param val The value to convert.
 * @return 0 on success, negative on error.
 */
YYIO_wur YYIO_nn()
int YYIO_astrfrom$1(YYIO_string *o, const struct yio_printfmt_s *pf, $2 val);

{% endcall %}
#endif

#endif // YYIO_HAS_STDFIX_TYPES
#endif /* YYIO_YIO_PRIVATE_YIO_STDFIX_STRFROM_H_ */
