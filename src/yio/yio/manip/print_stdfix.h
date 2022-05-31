/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_STDFIX_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_STDFIX_H_
#if YYIO_HAS_STDFIX_TYPES

{% from "yio/private/yio_stdfix.h" import j_STDFIX %}

{% call j_FOREACHAPPLY(j_STDFIX) %}
/// Print $2 type variable to yπio_printctx stream
int YYΩIO_print_$3(yπio_printctx_t *t);
{% endcall %}

#define YYΩIO_PRINT_STDFIX() \
		{% call j_FOREACHAPPLY(j_STDFIX) %}
		$2: YYΩIO_print_$3, \
		{% endcall %}
		/**/

#else // YYIO_HAS_STDFIX_TYPES
#define YYΩIO_PRINT_STDFIX()
#endif // YYIO_HAS_STDFIX_TYPES

#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_STDFIX_H_ */
