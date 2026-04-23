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
int yvbprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Similar to printf() call.
 * @see yvbprintf
 */
YYIO_nn(1, 3)
int yvprintf(const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yvbprintf
 */
YYIO_nn(1, 2, 4) YYIO_access_r(1) YYIO_access_r(2) YYIO_access_r(3) YYIO_access_r(4)
int yvfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Similar to snprintf() call.
 * @see yvbprintf
 */
YYIO_nn(1, 3, 5) YYIO_access_w(1) YYIO_access_r(3) YYIO_access_r(4) YYIO_access_r(5)
int yvsprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Works as-if the call to GNU extension asprintf().
 * Equal to doing: *strp = NULL; yvreasrintf(strp, ...);
 * @see yvreasprintf
 */
YYIO_nn(1, 2, 4)
int yvaprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Reallocates a string to append the printed characters.
 * @param strp If is NULL, a new string is allocated, otherwise it is reallocated.
 * @param data
 * @param va
 * @return Number of characters written or negative on error.
 *         In case of memory error the pointer strp is freed and is set to NULL.
 */
YYIO_nn(1, 2, 4)
int yvreaprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Calls yvreformatf(NULL, ...). See yvreformatf.
 * @see yvreformatf
 */
YYIO_retmalloc YYIO_nn(1, 3) YYIO_wur
char *yvformatf(const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Dynamically allocates memory for formatted string.
 *
 * The call of this function is similar to the python-ish and
 * C++ fmt::format calls. Basically this is a small wrapper around
 * yvreaprintf() call.
 *
 * @param str A pointer to dynamically allocated string of characters
 * @param data A pointer to Yio printing data.
 * @param va A pointer to va_list of arguments.
 * @return Returns a pointer to valid string of characters or NULL.
 *         In case of allocation error NULL is returned and
 *         <b> memory pointer to by str is also freed.</b>
 *         If you want to save the string, duplicate it before
 *         calling this function. *
 */
YYIO_nn(2, 4) YYIO_wur
char *yvreformatf(char *str, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * Output to the file descriptor. Similar to POSIX dprintf() call.
 * @see yvbprintf
 */
YYIO_nn(2, 4)
int yvdprintf(int fd, const yio_printdata_t *data, const char *fmt, va_list *va);
/**
 * @}
 */

/**
 * @defgroup YYIO_printf YYIO_printf
 * @ingroup yio
 * @brief Printing macro callbacks.
 * @see yvbprintf
 * @{
 */
YYIO_nn(1, 2)
int YYIO_ybprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1)
int YYIO_yprintf(const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 2)
int YYIO_yfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 3) YYIO_access_w(1)
int YYIO_ysprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 2)
int YYIO_yaprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1, 2)
int YYIO_yreaprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(1) YYIO_retmalloc YYIO_wur
char *YYIO_yformatf(const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(2) YYIO_wur
char *YYIO_yreformatf(char *str, const yio_printdata_t *data, const char *fmt, ...);
YYIO_nn(2)
int YYIO_ydprintf(int fd, const yio_printdata_t *data, const char *fmt, ...);
/**
 * @}
 */

/**
 * @defgroup y__printf y__printf
 * @ingroup yio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define ybprintf(cb, arg, ...)     YYIO_ybprintf(cb, arg, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yprintf(...)               YYIO_yprintf(YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yfprintf(file, ...)        YYIO_yfprintf(file, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define ysprintf(dest, size, ...)  YYIO_ysprintf(dest, size, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yaprintf(strp, ...)        YYIO_yaprintf(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yreaprintf(strp, ...)      YYIO_yreaprintf(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yformatf(...)              YYIO_yformatf(YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yreformatf(str, ...)       YYIO_yreformatf(str, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define ydprintf(fd, ...)          YYIO_ydprintf(fd, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup y__print y__print
 * @ingroup yio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define ybprint(cb, arg, ...)     YYIO_ybprintf(cb, arg, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yprint(...)               YYIO_yprintf(YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yfprint(file, ...)        YYIO_yfprintf(file, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define ysprint(dest, size, ...)  YYIO_ysprintf(dest, size, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yaprint(strp, ...)        YYIO_yaprintf(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yreaprint(strp, ...)      YYIO_yreaprintf(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yformat(...)              YYIO_yformatf(YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yreformat(str, ...)       YYIO_yreformatf(str, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define ydprint(fd, ...)          YYIO_ydprintf(fd, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/**
 * @def YIO_PRINT_ARGUMENTS
 * @ingroup yio
 *
 * Create argument list for yprint functions
 * If an argument doesn't has 62 commas, then the function for it is chosen
 *    using _Generic expression with @c YYIO_PRINT_FUNC_GENERIC.
 * If an argument does has 62 commas, then the first argument that should be inside braces is extracted
 *   and the first argument from inside the braces is the function to handle the arguments.
 * The arguments are appended to the end of the function.
 *
 * For example a call like:
 *    yprint(1, yiocb(function, 2, 3));
 * Is expanded to:
 *    yprint((const YYIO_func_t[]){ _Generic((1), int: YYIO_print_int), function, NULL }, 1, 2, 3);
 *
 * The first argument is an array of functions to handle arguments, delimitered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define YIO_PRINT_ARGUMENTS(...)  \
		YYIO_print_arguments_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})( \
				YYIO_PRINT_FUNC_GENERIC, yio_printdata_t, __VA_ARGS__)

#endif /* YYIO_YIO_YIO_IO_H_ */
