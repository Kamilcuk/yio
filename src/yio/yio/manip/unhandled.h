/**
 * @file
 * @date 2020-lip-29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_UNHANDLED_H_
#define YYIO_YIO_YΩIO_MANIP_UNHANDLED_H_
#include "../ctx.h"

// From https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#Common-Function-Attributes
// ... is used on a function declaration and a **call** to such a function is ...
// I need to actually call the function.

YYIO_error("Type is not handled")
// YYIO_deprecated("Type is not handled")
int (*YYΩIO_print_unhandled_type(void))(yπio_printctx_t *t);
#define YYΩIO_print_unhandled_type  (YYΩIO_print_unhandled_type())
#endif /* YYIO_YIO_YΩIO_MANIP_UNHANDLED_H_ */
