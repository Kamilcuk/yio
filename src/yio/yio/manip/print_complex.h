/**
 * @file
 * @date 2020-08-06
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_COMPLEX_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_COMPLEX_H_
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
int YYΩIO_print_complex_$1(yπio_printctx_t *t);
{% endcall %}
#define YYΩIO_PRINT_COMPLEX_COMPLEX() \
		{% call j_FOREACHAPPLY(j_complex_types) %}
		$2: YYΩIO_print_complex_$1, \
		{% endcall %}
		/**/

#if YYIO_HAS_IMAGINARY

{% call j_FOREACHAPPLY(j_imaginary_types) %}
#line
int YYΩIO_print_imaginary_$1(yπio_printctx_t *t);
{% endcall %}
#define YYΩIO_PRINT_COMPLEX_IMAGINARY() \
		{% call j_FOREACHAPPLY(j_imaginary_types) %}
		$2: YYΩIO_print_imaginary_$1, \
		{% endcall %}
		/**/

#else
#define YYΩIO_PRINT_COMPLEX_IMAGINARY()
#endif

#define YYΩIO_PRINT_COMPLEX() \
		YYΩIO_PRINT_COMPLEX_COMPLEX() \
		YYΩIO_PRINT_COMPLEX_IMAGINARY()

#else
#define YYΩIO_PRINT_COMPLEX()
#endif

#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_COMPLEX_H_ */
