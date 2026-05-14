/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_BOOL_H_
#define YIO_YIO_YIO_MANIP_PRINT_BOOL_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

int YIO_print_bool(yio_printctx_t *t);
int YIO_print_char(yio_printctx_t *t);
int YIO_print_constcharpnt(yio_printctx_t *t);
int YIO_print_voidp(yio_printctx_t *t);

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_BOOL_H_
