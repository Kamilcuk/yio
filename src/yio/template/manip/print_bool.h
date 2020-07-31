/*
 * print_bool.h
 *
 *  Created on: 19 cze 2020
 *      Author: kamil
 */
#include <stdbool.h>

int _yΩIO_print_bool(yπio_printctx_t *t);

#define _yΩIO_PRINT_FUNC_GENERIC_BOOL() \
	bool: _yΩIO_print_bool,

