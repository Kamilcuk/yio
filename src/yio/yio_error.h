/**
 * @file yio_error.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>

/**
 * Custom yio library errors.
 * From these errors using a regex messages in
 * yio_error_messages_gen.h are generated.
 * See yio_error_messages_gen.cmake script
 */
enum _yIO_errors_e {
	YIO_ERROR_OK = 0,
	YIO_ERROR_ENOMEM = -ENOMEM,    /// "Not enough memory", the same as ENOMEM
	YIO_ERROR_ENOSYS = -ENOSYS,    /// "Function not implemented", basically the same as ENOSYS
	YIO_ERROR_EIO = -EIO,          /// "Input/output error", means something wrong with FILE*
	YIO_ERROR_ENOBUFS = -ENOBUFS,  /// "No buffer space available", snprintf has nowhere to write to
	_yIO_ERROR_START = -2100,      /// Start of custom errors - 2100
	YIO_ERROR_FMT_INVALID,         /// GEN "Invalid format specifier"
	YIO_ERROR_PYFMT_INVALID,       /// GEN "Invalid python format specifier"
	YIO_ERROR_CFMT_INVALID,        /// GEN "Invalid C format specifier"
	YIO_ERROR_SCANFMT_INVALID,     /// GEN "Invalid scan format specifier"
	YIO_ERROR_SCAN_NOT_MATCHED,    /// GEN "Scanned character not matched"
	YIO_ERROR_SCAN_OVERFLOW,       /// GEN "Scanning overflowed"
	YIO_ERROR_SCAN_UNDERFLOW,      /// GEN "Scanning underflowed"
	YIO_ERROR_TOO_MANY_FMT,        /// GEN "Too many format strings for arguments"
	YIO_ERROR_SCAN_NOTHING,        /// GEN "No matching characters scanned"
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
#ifdef __GNUC__
__attribute__((__const__))
__attribute__((__returns_nonnull__))
#endif
const char *yio_strerror(int error);

#ifdef __cplusplus
}
#endif
