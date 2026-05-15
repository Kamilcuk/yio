/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_IO_H_
#define YIO_YIO_YIO_IO_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "ctx.h"
#include <stdio.h>
#include <stdarg.h>

#ifndef YIO_HAS_FILE
#error YIO_HAS_FILE is not defined
#endif

/* Exported Print Symbols --------------------------------------------------- */

/**
 * @defgroup yv__print yv__print
 * @ingroup yio
 * @brief Printing functions with va_list.
 * @{
 */
/**
 * The heart of this library. The function writes formatted data to a user-provided output callback.
 * It uses a pre-calculated yio_printdata_t array to determine how to format each argument.
 * 
 * @param out Callback function to write output (writes to a file, buffer, or network).
 * @param arg User-defined argument passed to the callback function (a FILE* or buffer pointer).
 * @param data Pointer to the pre-calculated printing context data array (terminated by NULL).
 * @param fmt The format string containing replacement fields like "{}".
 * @param va Pointer to the va_list containing the arguments to be formatted.
 * @return Number of characters written or a negative value on error (see yio_error.h).
 */
YIO_nn(1, 3, 5)
int yio_vbprintf(YIO_printcb_t *out, void *arg, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
/**
 * Similar to printf() call.
 * @see yio_vbprintf
 */
YIO_nn(1, 3)
int yio_vprintf(const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yio_vbprintf
 */
#if YIO_HAS_FILE
YIO_nn(1, 2, 4) YIO_access_r(1) YIO_access_r(2) YIO_access_r(3) YIO_access_r(4)
int yio_vfprintf(FILE *file, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
#endif
/**
 * Similar to snprintf() call.
 * @see yio_vbprintf
 */
YIO_nn(1, 3, 5) YIO_access_w(1) YIO_access_r(3) YIO_access_r(4) YIO_access_r(5)
int yio_vsprintf(char *__sized_by(size) dest, size_t size, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
#if YIO_ENABLE_MALLOC
/**
 * Works as-if the call to GNU extension asprintf().
 * If *strp is non-NULL, it will reuse the storage, assuming the allocated
 * size is exactly strlen(*strp).
 */
YIO_nn(1, 2, 4)
int yio_vasprintf(char **__single strp, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
/**
 * Appends the formatted string to the existing string storage.
 * 
 * If *strp is NULL, a new string is allocated. Otherwise, the existing string
 * is reallocated to accommodate the new content. The function assumes that
 * the current string was previously allocated via malloc/realloc and is
 * null-terminated.
 * 
 * @param strp Pointer to the string pointer. Updated on success/failure.
 * @param data Pre-calculated printing data.
 * @param fmt Format string.
 * @param va Variadic arguments.
 * @return Number of characters written or a negative value on error.
 *         IMPORTANT: In case of memory error (realloc failure), the original
 *         pointer *strp is freed and set to NULL to prevent leaks.
 */
YIO_nn(1, 2, 4)
int yio_vappend(char **__single strp, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
#endif // YIO_ENABLE_MALLOC
/**
 * Output to the file descriptor. Similar to POSIX dprintf() call.
 * @see yio_vbprintf
 */
YIO_nn(2, 4)
int yio_vdprintf(int fd, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, va_list *va);
/**
 * @}
 */

/**
 * @defgroup YIO_print YIO_print
 * @ingroup yio
 * @brief Printing macro callbacks.
 * @see yio_vbprintf
 * @{
 */
YIO_nn(1) YIO_FORMAT(4)
int YIO_yio_bprint(YIO_printcb_t *out, void *arg, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
YIO_FORMAT(2)
int YIO_yio_print(const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
#if YIO_HAS_FILE
YIO_nn(1) YIO_FORMAT(3)
int YIO_yio_fprint(FILE *file, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
#endif
YIO_nn(1) YIO_access_w(1) YIO_FORMAT(4)
int YIO_yio_snprint(char *__sized_by(size) dest, size_t size, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
#if YIO_ENABLE_MALLOC
YIO_nn(1) YIO_FORMAT(3)
int YIO_yio_asprint(char **__single strp, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
YIO_nn(1) YIO_FORMAT(3)
int YIO_yio_append(char **__single strp, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
#endif

int YIO_yio_dprint(int fd, const yio_printdata_t *__null_terminated data, const char *__null_terminated fmt, ...);
/**
 * @}
 */

/**
 * @defgroup yio__print yio__print
 * @ingroup yio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yio_vbprintf
 * @{
 */
#define yio_bprint(cb, arg, ...)     YIO_yio_bprint(cb, arg, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yio_print(...)               YIO_yio_print(YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#if YIO_HAS_FILE
#define yio_fprint(file, ...)        YIO_yio_fprint(file, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#endif
#define yio_snprint(dest, size, ...)  YIO_yio_snprint(dest, size, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#if YIO_ENABLE_MALLOC
#define yio_asprint(strp, ...)       YIO_yio_asprint(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yio_append(strp, ...)         YIO_yio_append(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#endif
#define yio_dprint(fd, ...)          YIO_yio_dprint(fd, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup yio__stream yio__stream
 * @ingroup yio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yio_vbprintf
 * @{
 */
#define yio_bstream(cb, arg, ...)    YIO_yio_bprint(cb, arg, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_stream(...)              YIO_yio_print(YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#if YIO_HAS_FILE
#define yio_fstream(file, ...)       YIO_yio_fprint(file, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#endif
#define yio_snstream(dest, size, ...)  YIO_yio_snprint(dest, size, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#if YIO_ENABLE_MALLOC
#define yio_asstream(strp, ...)       YIO_yio_asprint(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_appendstream(strp, ...)   YIO_yio_append(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#endif

#define yio_dstream(fd, ...)          YIO_yio_dprint(fd, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/**
 * @def YIO_PRINT_ARGUMENTS
 * @ingroup yio
 *
 * Create argument list for yio_stream functions
 * If an argument doesn't has 62 commas, then the function for it is chosen
 *    using _Generic expression with @c YIO_PRINT_FUNC_GENERIC.
 * If an argument does has 62 commas, then the first argument that should be inside braces is extracted
 *   and the first argument from inside the braces is the function to handle the arguments.
 * The arguments are appended to the end of the function.
 *
 * For example a call like:
 *    yio_stream(1, yio_callback(function, 2, 3));
 * Is expanded to:
 *    yio_stream((const YIO_func_t[]){ _Generic((1), int: YIO_print_int), function, NULL }, 1, 2, 3);
 *
 * The first argument is an array of functions to handle arguments, delimitered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define YIO_PRINT_ARGUMENTS(...)  \
		YIO_print_arguments_N(__VA_ARGS__, {{j_seqcomma(j_MAX_ARGS, 0)}})( \
				YIO_PRINT_FUNC_GENERIC, __VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_IO_H_
