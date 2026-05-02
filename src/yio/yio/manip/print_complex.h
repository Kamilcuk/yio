/**
 * @file
 * @date 2020-08-06
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_COMPLEX_H_
#define YYIO_YIO_YIO_MANIP_PRINT_COMPLEX_H_
#ifdef __cplusplus
extern "C" {
#endif

#if YYIO_HAS_COMPLEX

{% set j_complex_types = [
		["cf", "float _Complex", "f"],
		["cd", "double _Complex", ""],
		["cl", "long double _Complex", "l"],
	] %}
{% set j_imaginary_types = [
		["cif", "float _Imaginary", "f"],
		["cid", "double _Imaginary", ""],
		["cil", "long double _Imaginary", "l"],
	] %}

{% call j_FOREACHAPPLY(j_complex_types) %}
#line
int YYIO_print_complex_$1(yio_printctx_t *t);
{% endcall %}
#ifndef __cplusplus
#define YYIO_PRINT_COMPLEX_COMPLEX() \
		{% call j_FOREACHAPPLY(j_complex_types) %} \
		YYIO_OVERLOAD_TYPE_FUNC($2, YYIO_print_complex_$1) \
		{% endcall %} \
		/**/

#if YYIO_HAS_IMAGINARY

{% call j_FOREACHAPPLY(j_imaginary_types) %}
#line
int YYIO_print_imaginary_$1(yio_printctx_t *t);
{% endcall %}
#define YYIO_PRINT_COMPLEX_IMAGINARY() \
		{% call j_FOREACHAPPLY(j_imaginary_types) %} \
		YYIO_OVERLOAD_TYPE_FUNC($2, YYIO_print_imaginary_$1) \
		{% endcall %} \
		/**/

#else
#define YYIO_PRINT_COMPLEX_IMAGINARY()
#endif

#define YYIO_PRINT_COMPLEX() \
		YYIO_PRINT_COMPLEX_COMPLEX() \
		YYIO_PRINT_COMPLEX_IMAGINARY()
#else
#define YYIO_PRINT_COMPLEX()
#endif

#else
#define YYIO_PRINT_COMPLEX()
#endif

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_COMPLEX_H_
