/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_IO_H_
#define YYIO_YIO_YIO_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ctx.h"
#include <stdio.h>
#include <stdarg.h>

/* Exported Print Symbols --------------------------------------------------- */

/**
 * @defgroup yv__print yv__print
 * @ingroup yio
 * @brief Printing functions with va_list.
 * @{
 */
/**
 * The hearth of this library. The function write the data from
 * Yio printing data to the output function.
 * @param out Callback function to write output.
 * @param arg The argumnet passed to callback function.
 * @param data Pointer to printing context.
 * @param va Pointer to va_list.
 * @return Number of characters written or negative on error.
 */
YYIO_nn(1, 3, 5)
int yio_vbprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Similar to printf() call.
 * @see yio_vbprintf
 */
YYIO_nn(1, 3)
int yio_vprintf(const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yio_vbprintf
 */
YYIO_nn(1, 2, 4) YYIO_access_r(1) YYIO_access_r(2) YYIO_access_r(3) YYIO_access_r(4)
int yio_vfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Similar to snprintf() call.
 * @see yio_vbprintf
 */
YYIO_nn(1, 3, 5) YYIO_access_w(1) YYIO_access_r(3) YYIO_access_r(4) YYIO_access_r(5)
int yio_vsprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, va_list *va);
#if YIO_USE_MALLOC
/**
 * Works as-if the call to GNU extension asprintf().
 * If *strp is non-NULL, it will reuse the storage, assuming the allocated
 * size is exactly strlen(*strp).
 */
YYIO_nn(1, 2, 4)
int yio_vasprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Appends the formatted string to the existing string storage.
 * @param strp If is NULL, a new string is allocated, otherwise it is reallocated.
 * @param data
 * @param va
 * @return Number of characters written or negative on error.
 *         In case of memory error the pointer strp is freed and is set to NULL.
 */
YYIO_nn(1, 2, 4)
int yio_vappend(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va);
#endif // YIO_USE_MALLOC
/**
 * Output to the file descriptor. Similar to POSIX dprintf() call.
 * @see yio_vbprintf
 */
YYIO_nn(2, 4)
int yio_vdprintf(int fd, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * @}
 */

/**
 * @defgroup YYIO_printf YYIO_printf
 * @ingroup yio
 * @brief Printing macro callbacks.
 * @see yio_vbprintf
 * @{
 */
YYIO_nn(1, 2)
int YYIO_yio_bprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1)
int YYIO_yio_printf(const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 2)
int YYIO_yio_fprintf(FILE *file, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 3) YYIO_access_w(1)
int YYIO_yio_snprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, ...);
#if YIO_USE_MALLOC
YYIO_nn(1, 2)
int YYIO_yio_asprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 2)
int YYIO_yio_append(char **strp, const yio_printdata_t *data, const char *fmt, ...);
#endif
YYIO_nn(2)
int YYIO_yio_dprintf(int fd, const yio_printdata_t *data, const char *fmt, ...);
/**
 * @}
 */

/**
 * @defgroup yio__printf yio__printf
 * @ingroup yio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yio_vbprintf
 * @{
 */
#define yio_bprintf(cb, arg, ...)     YYIO_yio_bprintf(cb, arg, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yio_printf(...)               YYIO_yio_printf(YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yio_fprintf(file, ...)        YYIO_yio_fprintf(file, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yio_snprintf(dest, size, ...)  YYIO_yio_snprintf(dest, size, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#if YIO_USE_MALLOC
#define yio_asprintf(strp, ...)       YYIO_yio_asprintf(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yio_append_f(strp, ...)       YYIO_yio_append(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#endif
#define yio_dprintf(fd, ...)          YYIO_yio_dprintf(fd, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
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
#define yio_bprint(cb, arg, ...)     YYIO_yio_bprintf(cb, arg, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_print(...)               YYIO_yio_printf(YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_fprint(file, ...)        YYIO_yio_fprintf(file, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_sprint(dest, size, ...)  YYIO_yio_snprintf(dest, size, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#if YIO_USE_MALLOC
#define yio_asprint(strp, ...)        YYIO_yio_asprintf(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_append(strp, ...)         YYIO_yio_append(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#endif
#define yio_dprint(fd, ...)          YYIO_yio_dprintf(fd, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/**
 * @def YIO_PRINT_ARGUMENTS
 * @ingroup yio
 *
 * Create argument list for yio_print functions
 * If an argument doesn't has 62 commas, then the function for it is chosen
 *    using _Generic expression with @c YYIO_PRINT_FUNC_GENERIC.
 * If an argument does has 62 commas, then the first argument that should be inside braces is extracted
 *   and the first argument from inside the braces is the function to handle the arguments.
 * The arguments are appended to the end of the function.
 *
 * For example a call like:
 *    yio_print(1, yio_callback(function, 2, 3));
 * Is expanded to:
 *    yio_print((const YYIO_func_t[]){ _Generic((1), int: YYIO_print_int), function, NULL }, 1, 2, 3);
 *
 * The first argument is an array of functions to handle arguments, delimitered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define YIO_PRINT_ARGUMENTS(...)  \
		YYIO_print_arguments_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})( \
				YYIO_PRINT_FUNC_GENERIC, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* YYIO_YIO_YIO_IO_H_ */
