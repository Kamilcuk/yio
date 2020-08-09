/**
 * @file
 * @date 2020-08-06
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#ifdef _yIO_HAS_COMPLEX

m4_define(`m4_complex_types~, `m4_dnl;
		(cf, float _Complex, f),
		(cd, double _Complex, ),
		(cl, long double _Complex, l)
~)
m4_define(`m4_imaginary_types~, `m4_dnl;
		(cif, float _Imaginary, f),
		(cid, double _Imaginary, ),
		(cil, long double _Imaginary, l)
~)

m4_applyforeachdefine((m4_complex_types), `m4_dnl;
#line m4___line__
int _yIO_print_complex_$1(yπio_printctx_t *t);
~)m4_dnl;
#define _yIO_PRINT_COMPLEX_COMPLEX() \
		m4_applyforeachdefine((m4_complex_types),
		`$2: _yIO_print_complex_$1,~, ` \
		~)m4_dnl;

#ifdef _yIO_HAS_IMAGINARY

m4_applyforeachdefine((m4_imaginary_types), `m4_dnl;
#line m4___line__
int _yIO_print_imaginary_$1(yπio_printctx_t *t);
~)m4_dnl;
#define _yIO_PRINT_COMPLEX_IMAGINARY() \
		m4_applyforeachdefine((m4_imaginary_types),
		`$2: _yIO_print_imaginary_$1,~, ` \
		~)m4_dnl;

#else
#define _yIO_PRINT_COMPLEX_IMAGINARY()
#endif

#define _yIO_PRINT_COMPLEX() \
		_yIO_PRINT_COMPLEX_COMPLEX() \
		_yIO_PRINT_COMPLEX_IMAGINARY()

#else
#define _yIO_PRINT_COMPLEX()
#endif

