/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_ERROR_H_
#define YYIO_YIO_YIO_ERROR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "yio_config.h"

{% set j_ERRORS = [
		[ "YIO_ERROR_ENOMEM", "Not enough memory", ],
		[ "YIO_ERROR_ENOSYS", "Function not implemented", ],
		[ "YIO_ERROR_EIO", "Input/output error", ],
		[ "YIO_ERROR_ENOBUFS", "No buffer space available", ],
		[ "YIO_ERROR_FMT_INVALID", "Invalid format specifier", ],
		[ "YIO_ERROR_PYFMT_INVALID", "Invalid python format specifier", ],
		[ "YIO_ERROR_CFMT_INVALID", "Invalid C format specifier", ],
		[ "YIO_ERROR_TOO_MANY_FMT", "Too many format strings for arguments", ],
		[ "YIO_ERROR_UNKNOWN_CONVERSION", "Unknown conversion specifier", ],
		[ "YIO_ERROR_UNKNOWN_FMT", "Unknown format code specifier for given type", ],
		[ "YIO_ERROR_WCRTOMB", "wcrtomb returned -1", ],
		[ "YIO_ERROR_MBRTOWC", "mbrtowc returned -1", ],
		[ "YIO_ERROR_MBRTOWC_2", "mbrtowc returned -2", ],
		[ "YIO_ERROR_U32_CONV_TO_ENCODING", "u32_conv_to_encoding returned NULL", ],
		[ "YIO_ERROR_U16_CONV_TO_ENCODING", "u16_conv_to_encoding returned NULL", ],
		[ "YIO_ERROR_U32_CONV_FROM_ENCODING", "u32_conv_from_encoding returned NULL", ],
		[ "YIO_ERROR_U16_CONV_FROM_ENCODING", "u16_conv_from_encoding returned NULL", ],
		[ "YIO_ERROR_C32RTOMB", "c32rtomb returned -1", ],
		[ "YIO_ERROR_C16RTOMB", "c16rtomb returned -1", ],
		[ "YIO_ERROR_SKIPPING", "error part of skipping positional arguments when iterating over them", ],
		[ "YIO_ERROR_POSITIONAL_DISABLED", "positional support is disabled, but used in format string", ],
		[ "YIO_ERROR_NOT_NUMBER", "width or precision positional parameter is not a number", ],
		[ "YIO_ERROR_INVALID_TYPE", "type specifier was invalid for given type", ],
		[ "YIO_ERROR_MBRTOC32", "mbrtoc32 returned error" ],
		[ "YIO_ERROR_MBRTOC32_2", "mbrtoc32 returned -2" ],
		[ "YIO_ERROR_MBRTOC32_3", "mbrtoc32 returned -3" ],
		[ "YIO_ERROR_MBRTOC16", "mbrtoc16 returned error" ],
		[ "YIO_ERROR_MBRTOC16_2", "mbrtoc16 returned -2" ],
		[ "YIO_ERROR_MBRTOC16_3", "mbrtoc16 returned -3" ],
	]
%}
#line

/**
 * Custom yio library errors.
 * From these errors using a regex messages in
 * yio_error_messages_gen.h are generated.
 * See yio_error_messages_gen.cmake script
 */
enum YYIO_errors_e {
	YIO_ERROR_OK = 0,              /// "Success"
	YYIO_ERROR_START = -12300,     /// Start of custom errors
{% for v, k in j_ERRORS %} #line
	{{ v }},  /// {{ k }}
{% endfor %} #line
#include "yio_error_genenum.h"
	YYIO_ERROR_STOP,               /// End of custom errors
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
YYIO_const YYIO_rnn
const char *yio_strerror(int error);

#ifdef __cplusplus
}
#endif
#endif /* YYIO_YIO_YIO_ERROR_H_ */
