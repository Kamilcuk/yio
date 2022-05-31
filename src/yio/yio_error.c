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
 * that correspond to enumeration in enum YYIO_errors_e
 */
static const char *const YYIO_error_messages[] = {
{% from "yio/yio_error.h" import j_ERRORS %}
{% for v, k in j_ERRORS %} #line
	"{{ k }}",  // {{ v }}
{% endfor %} #line
#include "yio_error_genmsg.h"
};

static const char unknown_error[] = "unknown error";

const char *yio_strerror(int error) {
	static_assert(sizeof(YYIO_error_messages)/sizeof(YYIO_error_messages[0]) > 1, "");
	static_assert(sizeof(YYIO_error_messages)/sizeof(YYIO_error_messages[0]) < 100, "");
	static_assert(sizeof(YYIO_error_messages)/sizeof(YYIO_error_messages[0]) ==
			YYIO_ERROR_STOP - YYIO_ERROR_START - 1, "");

	static_assert(YYIO_ERROR_START < YYIO_ERROR_STOP, "");
	if (!(YYIO_ERROR_START < error && error < YYIO_ERROR_STOP)) {
		return unknown_error;
	}
	error -= (YYIO_ERROR_START + 1);
	assert(0 <= error);
	assert(error < (int)(sizeof(YYIO_error_messages)/sizeof(YYIO_error_messages[0])));
	return YYIO_error_messages[error];
}

