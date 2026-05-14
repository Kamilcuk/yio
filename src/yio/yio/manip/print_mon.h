/**
 * @file
 * @date 2022-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_MON_H_
#define YIO_YIO_YIO_MANIP_PRINT_MON_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx.h"

#ifndef YIO_HAS_MONETARY_H
#error
#endif
#if YIO_HAS_MONETARY_H

int YIO_print_mon(yio_printctx_t *t);

#define yio_mon(doublev)  yio_callback(YIO_print_mon, _Generic((doublev), double: (doublev)))

#endif

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_MON_H_
