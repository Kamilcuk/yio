/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_COUNT_H_
#define YYIO_YIO_YIO_MANIP_PRINT_COUNT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

int YYIO_print_count(yio_printctx_t *t);

#ifdef __cplusplus
#define yio_count(v)  yio_callback(YYIO_print_count, (v))
#else
#define yio_count(v)  yio_callback(YYIO_print_count, _Generic((v),int *:(v)))
#endif

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_COUNT_H_
