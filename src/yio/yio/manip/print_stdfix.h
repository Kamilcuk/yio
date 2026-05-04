/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_STDFIX_H_
#define YYIO_YIO_YIO_MANIP_PRINT_STDFIX_H_
#ifdef __cplusplus
extern "C" {
#endif

#if YYIO_HAS_STDFIX_TYPES

{% call j_FOREACHAPPLY(j_STDFIX) %}#line
#ifndef YYIO_HAS_STDFIX_$3
#error YYIO_HAS_STDFIX_$3 is not defined
#endif
#if YYIO_HAS_STDFIX_$3
/// Print $2 type variable to yio_printctx stream
int YYIO_print_$3(yio_printctx_t *t);
#endif
{% endcall %}#line

#ifndef __cplusplus
#define YYIO_PRINT_STDFIX() \
		{% call j_FOREACHAPPLY(j_STDFIX) %}
		YYIO_IF(YYIO_HAS_STDFIX_$3, YYIO_OVERLOAD_TYPE_FUNC($2, YYIO_print_$3)) \
		{% endcall %}
		/**/
#else
#define YYIO_PRINT_STDFIX()
#endif

#else // YYIO_HAS_STDFIX_TYPES
#define YYIO_PRINT_STDFIX()
#endif // YYIO_HAS_STDFIX_TYPES

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_STDFIX_H_
