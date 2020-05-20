/**
 * @file yio_error.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_error.h"
#include "yio_error_messages.h"
#include "yio_static_assert.h"
#include <string.h>
#include <stdio.h>

static const char unknown_error[] = "unknown error";

const char *yio_strerror(int error) {
#ifdef __GNUC__
	static_assert(__builtin_types_compatible_p(
			__typeof__(_yIO_error_messages), __typeof__(&_yIO_error_messages[0])) == 0, "");
#endif
	static_assert(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0]) > 1, "");
	static_assert(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0]) < 100, "");
	static_assert(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0]) ==
			_yIO_ERROR_STOP - _yIO_ERROR_START - 1, "");
	switch (error) {
	case YIO_ERROR_ENOMEM: return "Not enough memory";
	case YIO_ERROR_ENOSYS: return "Function not implemented";
	case YIO_ERROR_EIO: return "Input/output error";
	}
	static_assert(_yIO_ERROR_START < _yIO_ERROR_STOP, "");
	if (_yIO_ERROR_STOP <= error || error <= _yIO_ERROR_START) {
		return unknown_error;
	}
	error -= (_yIO_ERROR_START + 1);
	assert(0 <= error);
	assert(error < (int)(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0])));
	return _yIO_error_messages[error];
}

