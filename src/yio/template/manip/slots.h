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
/**
 * @def YΩIO_SCAN_SLOT_100
 * User define-able macro that can be used to provide
 * an user overload for specific types.
 * @see YΩIO_PRINT_SLOT_100
 */

m4_applyforloopdefine(100, m4_SLOTS_END, «m4_dnl;
#ifndef YΩIO_PRINT_SLOT_$1
#define YΩIO_PRINT_SLOT_$1
#endif
#ifndef YΩIO_SCAN_SLOT_$1
#define YΩIO_SCAN_SLOT_$1
#endif
»)m4_dnl;


/**
 * Expanded all slots macros.
 */
#define _yΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		m4_applyforloopdefine(100, m4_SLOTS_END, «YΩIO_PRINT_SLOT_$1», « »)

/**
 * Expanded all slots macros.
 */
#define _yΩIO_SCAN_FUNC_GENERIC_SLOTS() \
		m4_applyforloopdefine(100, m4_SLOTS_END, «YΩIO_SCAN_SLOT_$1», « »)


