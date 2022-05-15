/**
 * @file
 * @date 2022-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_MANIP_PRINT_MON_H_
#define _yIO_YIO_YΩIO_MANIP_PRINT_MON_H_
#include "../ctx.h"

#ifndef _yIO_HAS_MONETARY_H
#error
#endif
#if _yIO_HAS_MONETARY_H

int _yΩIO_print_mon(yπio_printctx_t *t);

#define yπpmon(doublev)  yiocb(_yΩIO_print_mon, _Generic((doublev), double: (doublev)))

#endif

#endif /* _yIO_YIO_YΩIO_MANIP_PRINT_MON_H_ */
