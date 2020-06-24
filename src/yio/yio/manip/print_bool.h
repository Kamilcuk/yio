/*
 * print_bool.h
 *
 *  Created on: 19 cze 2020
 *      Author: kamil
 */
m4_config(yio);
#include <stdbool.h>

int _yIO_print_bool(yio_printctx_t *t);

#define _yIO_PRINT_FUNC_GENERIC_BOOL() \
	bool: _yIO_print_bool,
