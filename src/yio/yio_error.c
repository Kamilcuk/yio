/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_error.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/**
 * A static list of error messages
 * that correspond to enumeration in enum YIO_errors_e
 */
static const char *const YIO_error_messages[] = {
{% from "yio/yio_error.h" import j_ERRORS %}
{% for v, k in j_ERRORS %} #line
	"{{ k }}",  // {{ v }}
{% endfor %} #line
#include "yio_error_genmsg.h"
};

static const char unknown_error[] = "unknown error";

const char *yio_strerror(int error) {
	static_assert(sizeof(YIO_error_messages)/sizeof(YIO_error_messages[0]) > 1, "");
	static_assert(sizeof(YIO_error_messages)/sizeof(YIO_error_messages[0]) < 100, "");
	static_assert(sizeof(YIO_error_messages)/sizeof(YIO_error_messages[0]) ==
			YIO_ERROR_STOP - YIO_ERROR_START - 1, "");

	static_assert(YIO_ERROR_START < YIO_ERROR_STOP, "");
	if (!(YIO_ERROR_START < error && error < YIO_ERROR_STOP)) {
		return unknown_error;
	}
	error -= (YIO_ERROR_START + 1);
	assert(0 <= error);
	assert(error < (int)(sizeof(YIO_error_messages)/sizeof(YIO_error_messages[0])));
	return YIO_error_messages[error];
}

