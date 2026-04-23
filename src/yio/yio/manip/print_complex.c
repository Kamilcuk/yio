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
#if YYIO_HAS_COMPLEX
#include <complex.h>

#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ == 11
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105331
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

{% from "yio/yio/manip/print_complex.h" import j_complex_types, j_imaginary_types %}

{% call j_FOREACHAPPLY(j_complex_types) %}
#line
int YYIO_print_complex_$1(yio_printctx_t *t) {
	const $2 val = yio_printctx_va_arg(t, $2); // NOLINT(clang-analyzer-valist.Uninitialized)
	int err = yio_printctx_init(t);
	if (err) return err;
	return yio_printctx_printf(t, "{}{:+}i", creal$3(val), cimag$3(val));
}
{% endcall %}

#if YYIO_HAS_IMAGINARY
{% call j_FOREACHAPPLY(j_imaginary_types) %}
#line
int YYIO_print_complex_$1(yio_printctx_t *t) {
	const $2 val = yio_printctx_va_arg(t, $2);
	int err = yio_printctx_init(t);
	if (err) return err;
	return yio_printctx_printf(t, "0{:+}i", cimag$3(val));
}
{% endcall %}
#endif // YYIO_HAS_IMAGINARY

#endif
