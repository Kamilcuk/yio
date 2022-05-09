/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

/**
 * @def YΩIO_PRINT_SLOT_100
 * User define-able macro that can be used to provide
 * an user overload for specific types.
 * To overflow on a type use a part of @b _Generic() syntax.
 * For example if you want to provide a custom overflow
 * for some custom type, it could look like this:
 *
 *      #define YΩIO_PRINT_SLOT_101 struct A: πprint_struct_a,
 *      #include <yπio.h>
 *
 *      struct A {
 *         int a;
 *         int b;
 *      };
 *      int πprint_struct_a(yπio_printctx_t *ctx) {
 *          struct A printme = yπio_printctx_va_arg(t, struct A);
 *          return yπio_printctx_print(printme.a, ", ", printme.b);
 *      }
 *      int main() {
 *         struct A a = { 1, 2 };
 *         yπprint(a); // will print "1, 2"
 *      }
 *
 */

{% if j_SLOTS|int %}{% call j_FOREACHAPPLY(j_range(100, 100 + j_SLOTS|int)) %}
#ifndef YΩIO_PRINT_SLOT_$1
#define YΩIO_PRINT_SLOT_$1
#endif
{% endcall %}{% endif %}


/**
 * Expanded all slots macros.
 */
#define _yΩIO_PRINT_FUNC_GENERIC_SLOTS()  \
		{% if j_SLOTS|int %}{% call j_FOREACHAPPLY(j_range(100, 100 + j_SLOTS|int)) %}
		YΩIO_PRINT_SLOT_$1  \
		{% endcall %}{% endif %}
		/* */

