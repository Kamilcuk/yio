/**
 * @file
 * @date 2020-08-06
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#if _yIO_HAS_COMPLEX

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
int _yΩIO_print_complex_$1(yπio_printctx_t *t);
{% endcall %}
#define _yΩIO_PRINT_COMPLEX_COMPLEX() \
		{% call j_FOREACHAPPLY(j_complex_types) %}
		$2: _yΩIO_print_complex_$1, \
		{% endcall %}
		/**/

#if _yIO_HAS_IMAGINARY

{% call j_FOREACHAPPLY(j_imaginary_types) %}
#line
int _yΩIO_print_imaginary_$1(yπio_printctx_t *t);
{% endcall %}
#define _yΩIO_PRINT_COMPLEX_IMAGINARY() \
		{% call j_FOREACHAPPLY(j_imaginary_types) %}
		$2: _yΩIO_print_imaginary_$1, \
		{% endcall %}
		/**/

#else
#define _yΩIO_PRINT_COMPLEX_IMAGINARY()
#endif

#define _yΩIO_PRINT_COMPLEX() \
		_yΩIO_PRINT_COMPLEX_COMPLEX() \
		_yΩIO_PRINT_COMPLEX_IMAGINARY()

#else
#define _yΩIO_PRINT_COMPLEX()
#endif

