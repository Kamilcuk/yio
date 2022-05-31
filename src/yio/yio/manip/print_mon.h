/**
 * @file
 * @date 2022-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_MON_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_MON_H_
#include "../ctx.h"

#ifndef YYIO_HAS_MONETARY_H
#error
#endif
#if YYIO_HAS_MONETARY_H

int YYΩIO_print_mon(yπio_printctx_t *t);

#define yπpmon(doublev)  yiocb(YYΩIO_print_mon, _Generic((doublev), double: (doublev)))

#endif

#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_MON_H_ */
