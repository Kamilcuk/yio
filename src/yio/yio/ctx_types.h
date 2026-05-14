/**
 * @file
 * @date 2026-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_CTX_TYPES_H_
#define YIO_YIO_YIO_CTX_TYPES_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#include <stddef.h>

/**
 * Print context.
 */
typedef struct YIO_printctx_s yio_printctx_t;

/**
 * The type representing a callback that will write output data to the user specified place.
 * @param arg Custom argument passed with callback registration.
 * @param data The pointer to the data to print.
 * @param count Count of characters to print
 * @return 0 on success, otherwise error
 */
typedef int (YIO_printcb_t)(void *arg, const char * __sized_by(count) data, size_t count)
		YIO_wur YIO_nn(2) YIO_access_r(2, 3) YIO_REENTRANT;

/**
 * The type of callback functions, but abstractly, this represents
 * the "printing data" that are needed to represent the printing context.
 * Callback functions need to follow a contract, in order:
 * 1. Firstly, they need to eat the argument using @c yio_printctx_va_arg.
 * 2. Optionally, then they need to parse the format string up until a '}'.
 * 3. Then, they need to call @c yio_printctx_init.
 * 4. If _init returns non-zero, the value has to be returned.
 * 5. Then, there is a place for custom handling.
 * 6. The function should call @c yio_printctx_put* functions and return with it's return value.
 * This contact is really important for making positional arguments work properly.
 * @param t Printing context.
 * @return 0 on success, otherwise error.
 */
typedef int (*yio_printdata_t)(yio_printctx_t * __single t) YIO_REENTRANT;

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_CTX_TYPES_H_
