/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

int _yIO_print_count(yio_printctx_t *t);

#define ypcount(v)  yiocb(_yIO_print_count, _Generic((v),int *:(v)))

#define _yIO_PRINT_FUNC_GENERIC_COUNT() \
		int *: _yIO_print_count,
