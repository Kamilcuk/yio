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
#if _yIO_HAS_COMPLEX
#include <complex.h>

#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ == 11
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105331
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

{% from "yio/template/manip/print_complex.h" import j_complex_types, j_imaginary_types %}

{% call j_FOREACHAPPLY(j_complex_types) %}
#line
int _yΩIO_print_complex_$1(yπio_printctx_t *t) {
	const $2 val = yπio_printctx_va_arg(t, $2); // NOLINT(clang-analyzer-valist.Uninitialized)
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, TC("{}{:+}i"), creal$3(val), cimag$3(val));
}
{% endcall %}

#if _yIO_HAS_IMAGINARY
{% call j_FOREACHAPPLY(j_imaginary_types) %}
#line
int _yΩIO_print_complex_$1(yπio_printctx_t *t) {
	const $2 val = yπio_printctx_va_arg(t, $2);
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, TC("0{:+}i"), cimag$3(val));
}
{% endcall %}
#endif // _yIO_HAS_IMAGINARY

#endif
