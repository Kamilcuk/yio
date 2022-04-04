/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "yio_common.h"
#include <errno.h>

/**
 * Custom yio library errors.
 * From these errors using a regex messages in
 * yio_error_messages_gen.h are generated.
 * See yio_error_messages_gen.cmake script
 */
enum _yIO_errors_e {
	YIO_ERROR_OK = 0,              /// "Success"
	_yIO_ERROR_START = -12300,     /// Start of custom errors
{% from "yio/yio_error.jinja" import ERR %}
{% for v, k in ERR %}
	{{ v }},
{% endfor %}
	_yIO_ERROR_STOP,               /// End of custom errors
};

/**
 * Returns the error string that corresponds to the number given.
 * It doesn't call the standard errno strerror() function,
 * which can dynamically allocate memory.
 * In case of unknown error code, this function
 * will return a string "unknown error".
 * @param error Error number.
 * @return The function never returns null and always returns
 * a valid pointer to a string with static storage duration.
 */
_yIO_const _yIO_rnn
const char *yio_strerror(int error);

#ifdef __cplusplus
}
#endif
