/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_BOOL_H_
#define YYIO_YIO_YIO_MANIP_PRINT_BOOL_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

int YYIO_print_bool(yio_printctx_t *t);
int YYIO_print_char(yio_printctx_t *t);
int YYIO_print_constcharpnt(yio_printctx_t *t);
int YYIO_print_voidp(yio_printctx_t *t);

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_BOOL_H_
