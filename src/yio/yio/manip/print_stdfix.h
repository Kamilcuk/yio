/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_STDFIX_H_
#define YIO_YIO_YIO_MANIP_PRINT_STDFIX_H_
#ifdef __cplusplus
extern "C" {
#endif

#if YIO_HAS_STDFIX_TYPES

{% call j_FOREACHAPPLY(j_STDFIX) %}#line
#ifndef YIO_HAS_STDFIX_$3
#error YIO_HAS_STDFIX_$3 is not defined
#endif
#if YIO_HAS_STDFIX_$3
/// Print $2 type variable to yio_printctx stream
int YIO_print_$3(yio_printctx_t *t);
#endif
{% endcall %}#line

#ifndef __cplusplus
#define YIO_PRINT_STDFIX(X, XALIAS) \
		{% call j_FOREACHAPPLY(j_STDFIX) %} \
		YIO_IF(YIO_HAS_STDFIX_$3, X($2, YIO_print_$3)) \
		{% endcall %} \
		/**/
#else
#define YIO_PRINT_STDFIX(X, XALIAS)
#endif

#else // YIO_HAS_STDFIX_TYPES
#define YIO_PRINT_STDFIX(X, XALIAS)
#endif // YIO_HAS_STDFIX_TYPES


#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_STDFIX_H_
