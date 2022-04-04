/**
 * @file
 * @date 2020-lip-29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

// From https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#Common-Function-Attributes
// ... is used on a function declaration and a **call** to such a function is ...
// I need to actually call the function.

_yIO_error("Type is not handled")
// _yIO_deprecated("Type is not handled")
int (*_yΩIO_print_unhandled_type(void))(yπio_printctx_t *t);
#define _yΩIO_print_unhandled_type  (_yΩIO_print_unhandled_type())
