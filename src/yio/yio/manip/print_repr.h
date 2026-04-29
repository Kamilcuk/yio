/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_REPR_H_
#define YYIO_YIO_YIO_MANIP_PRINT_REPR_H_
#include "../ctx_types.h"

#define yio_repr           yio_repr_hex
int YYIO_print_repr_hex(yio_printctx_t *t);
#define yio_repr_hex(x)    yio_callback(YYIO_print_repr_hex, 0), (x)
int YYIO_print_repr_oct(yio_printctx_t *t);
#define yio_repr_oct(x)    yio_callback(YYIO_print_repr_oct, 0), (x)
int YYIO_print_repr_caret(yio_printctx_t *t);
#define yio_repr_caret(x)  yio_callback(YYIO_print_repr_caret, 0), (x)
int YYIO_print_repr_u(yio_printctx_t *t);
#define yio_repr_u(x)      yio_callback(YYIO_print_repr_u, 0), (x)
int YYIO_print_repr_U(yio_printctx_t *t);
#define yio_repr_U(x)      yio_callback(YYIO_print_repr_U, 0), (x)

/**
 * @}
 */
#endif /* YYIO_YIO_YIO_MANIP_PRINT_REPR_H_ */
