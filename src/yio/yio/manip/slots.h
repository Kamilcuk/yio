/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config(yio);

/**
 * @def YIO_PRINT_SLOT_100
 * User define-able macro that can be used to provide
 * an user overload for specific types.
 * To overflow on a type use a part of @b _Generic() syntax.
 * For example if you want to provide a custom overflow
 * for some custom type, it could look like this:
 *
 *      #define YIO_PRINT_SLOT_101() struct A: print_struct_a,
 *      #include <yio.h>
 *
 *      struct A {
 *         int a;
 *         int b;
 *      };
 *      int print_struct_a(yio_printctx_t *ctx) {
 *          struct A printme = yio_printctx_va_arg(t, struct A);
 *          return yio_printctx_print(printme.a, ", ", printme.b);
 *      }
 *      int main() {
 *         struct A a = { 1, 2 };
 *         yprint(a); // will print "1, 2"
 *      }
 *
 */
/**
 * @def YIO_SCAN_SLOT_100
 * User define-able macro that can be used to provide
 * an user overload for specific types.
 * @see YIO_PRINT_SLOT_$1
 */

m4_applyforloopdefine(100, m4_SLOTS_END, `m4_dnl;
#ifndef YIO_PRINT_SLOT_$1
#define YIO_PRINT_SLOT_$1()
#endif
#ifndef YIO_SCAN_SLOT_$1
#define YIO_SCAN_SLOT_$1()
#endif
~)m4_dnl;


/**
 * Expanded all slots macros.
 */
#define _yIO_PRINT_FUNC_GENERIC_SLOTS() \
		m4_applyforloopdefine(100, m4_SLOTS_END, `YIO_PRINT_SLOT_$1()~)

/**
 * Expanded all slots macros.
 */
#define _yIO_SCAN_FUNC_GENERIC_SLOTS() \
		m4_applyforloopdefine(100, m4_SLOTS_END, `YIO_SCAN_SLOT_$1()~)


