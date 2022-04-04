/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

int _yΩIO_print_count(yπio_printctx_t *t);

#define yπpcount(v)  yiocb(_yΩIO_print_count, _Generic((v),int *:(v)))

#define _yΩIO_PRINT_FUNC_GENERIC_COUNT() \
		int *: _yΩIO_print_count,
