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

{% set j_ERRORS = [
		[ "YIO_ERROR_ENOMEM", "Not enough memory", ],
		[ "YIO_ERROR_ENOSYS", "Function not implemented", ],
		[ "YIO_ERROR_EIO", "Input/output error", ],
		[ "YIO_ERROR_ENOBUFS", "No buffer space available", ],
		[ "YIO_ERROR_FMT_INVALID", "Invalid format specifier", ],
		[ "YIO_ERROR_SINGLE_RIGHT_BRACE", "Single '}' encountered in format string", ],
		[ "YIO_ERROR_SINGLE_LEFT_BRACE", "Single '{' encountered in format string", ],
		[ "YIO_ERROR_EOF_IN_FMT", "end of string while looking for conversion specifier", ],
		[ "YIO_ERROR_PYFMT_INVALID", "Invalid python format specifier", ],
		[ "YIO_ERROR_CFMT_INVALID", "Invalid C format specifier", ],
		[ "YIO_ERROR_TOO_MANY_FMT", "Too many format strings for arguments", ],
		[ "YIO_ERROR_UNKNOWN_CONVERSION", "Unknown conversion specifier", ],
		[ "YIO_ERROR_UNKNOWN_FMT", "Unknown format code specifier for given type", ],
		[ "YIO_ERROR_STRFTIME", "strftime returned 0 and set errno", ],
		[ "YIO_ERROR_MISSING_PRECISION", "Format specifier missing precision", ],
		[ "YIO_ERROR_MISSING_RIGHT_BRACE", "expected '}' before end of string", ],
		[ "YIO_ERROR_WCTOMB", "wctomb returned -1", ],
		[ "YIO_ERROR_MBTOWC", "mbtowc returned -1", ],
		[ "YIO_ERROR_U32_CONV_TO_ENCODING", "u32_conv_to_encoding returned NULL", ],
		[ "YIO_ERROR_U16_CONV_TO_ENCODING", "u16_conv_to_encoding returned NULL", ],
		[ "YIO_ERROR_U32_CONV_FROM_ENCODING", "u32_conv_from_encoding returned NULL", ],
		[ "YIO_ERROR_U16_CONV_FROM_ENCODING", "u16_conv_from_encoding returned NULL", ],
		[ "YIO_ERROR_C32TOMB", "c32rtomb returned -1", ],
		[ "YIO_ERROR_C16TOMB", "c16rtomb returned -1", ],
		[ "YIO_ERROR_MBTOC32", "c32rtomb returned -1", ],
		[ "YIO_ERROR_MBTOC16", "c16rtomb returned -1", ],
		[ "YIO_ERROR_STRFROM", "strfrom returned -1", ],
	]
%}
#line

/**
 * Custom yio library errors.
 * From these errors using a regex messages in
 * yio_error_messages_gen.h are generated.
 * See yio_error_messages_gen.cmake script
 */
enum _yIO_errors_e {
	YIO_ERROR_OK = 0,              /// "Success"
	_yIO_ERROR_START = -12300,     /// Start of custom errors
{% for v, k in j_ERRORS %}
#line
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
