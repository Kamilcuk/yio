/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_error.h"
#include "yio_static_assert.h"
#include <string.h>
#include <stdio.h>

/**
 * A static list of error messages 
 * that correspond to enumeration in enum _yIO_errors_e
 */
static const char *const _yIO_error_messages[] = {
{% from "library.jinja" import j_ERRORS %}
{% for v, k in j_ERRORS %}
#line
	"{{ k }}",
{% endfor %}
};

static const char unknown_error[] = "unknown error";

const char *yio_strerror(int error) {
	static_assert(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0]) > 1, "");
	static_assert(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0]) < 100, "");
	static_assert(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0]) ==
			_yIO_ERROR_STOP - _yIO_ERROR_START - 1, "");

	static_assert(_yIO_ERROR_START < _yIO_ERROR_STOP, "");
	if (!(_yIO_ERROR_START < error && error < _yIO_ERROR_STOP)) {
		return unknown_error;
	}
	error -= (_yIO_ERROR_START + 1);
	assert(0 <= error);
	assert(error < (int)(sizeof(_yIO_error_messages)/sizeof(_yIO_error_messages[0])));
	return _yIO_error_messages[error];
}

