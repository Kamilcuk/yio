/**
 * @file
 * @date 2022-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_MON_H_
#define YYIO_YIO_YIO_MANIP_PRINT_MON_H_
#include "../ctx.h"

#ifndef YYIO_HAS_MONETARY_H
#error
#endif
#if YYIO_HAS_MONETARY_H

int YYIO_print_mon(yio_printctx_t *t);

#define yio_mon(doublev)  yio_callback(YYIO_print_mon, _Generic((doublev), double: (doublev)))
/**
 * Short versions
 */
#define ymon   yio_mon
#define ypmon  yio_mon


#endif


#endif /* YYIO_YIO_YIO_MANIP_PRINT_MON_H_ */
