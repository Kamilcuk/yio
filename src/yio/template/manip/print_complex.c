/**
 * @file
 * @date 2020-08-06
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "print_complex.h"
#ifdef _yIO_HAS_COMPLEX
#include <complex.h>

m4_divert(-1)
m4_include(`yio/template/manip/print_complex.h~)
m4_divert(0)

m4_applyforeachdefine((m4_complex_types()), `m4_dnl;
int _yΩIO_print_complex_$1(yπio_printctx_t *t) {
	const $2 val = yπio_printctx_va_arg(t, $2);
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, Yc("{}{:+}i"), creal$3(val), cimag$3(val));
}
~)m4_dnl;

#ifdef _yIO_HAS_IMAGINARY
m4_applyforeachdefine((m4_imaginary_types()), `m4_dnl;
int _yΩIO_print_complex_$1(yπio_printctx_t *t) {
	const $2 val = yπio_printctx_va_arg(t, $2);
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, Yc("0{:+}i"), cimag$3(val));
}
~)m4_dnl;
#endif // _yIO_HAS_IMAGINARY

#endif
