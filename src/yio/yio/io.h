/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_IO_H_
#define YYIO_YIO_YΩIO_IO_H_
#include "ctx.h"
#include <stdio.h>
#include <stdarg.h>

/* Exported Print Symbols --------------------------------------------------- */

/**
 * @defgroup yπv__print TMODEN yπv__print
 * @ingroup yπio
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
int yπvbprintf(YYΩIO_printcb_t *out, void *arg, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Similar to printf() call.
 * @see yvbprintf
 */
YYIO_nn(1, 3)
int yπvprintf(const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yvbprintf
 */
YYIO_nn(1, 2, 4) YYIO_access_r(1) YYIO_access_r(2) YYIO_access_r(3) YYIO_access_r(4)
int yπvfprintf(FILE *file, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Similar to snprintf() call.
 * @see yvbprintf
 */
YYIO_nn(1, 3, 5) YYIO_access_w(1) YYIO_access_r(3) YYIO_access_r(4) YYIO_access_r(5)
int yπvsprintf(TCHAR *dest, size_t size, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Works as-if the call to GNU extension asprintf().
 * Equal to doing: *strp = NULL; yvreasrintf(strp, ...);
 * @see yvreasprintf
 */
YYIO_nn(1, 2, 4)
int yπvaprintf(TCHAR **strp, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Reallocates a string to append the printed characters.
 * @param strp If is NULL, a new string is allocated, otherwise it is reallocated.
 * @param data
 * @param va
 * @return Number of characters written or negative on error.
 *         In case of memory error the pointer strp is freed and is set to NULL.
 */
YYIO_nn(1, 2, 4)
int yπvreaprintf(TCHAR **strp, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Calls yπvreformatf(NULL, ...). See yπvreformatf.
 * @see yπvreformatf
 */
YYIO_retmalloc YYIO_nn(1, 3) YYIO_wur
TCHAR *yπvformatf(const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
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
TCHAR *yπvreformatf(TCHAR *str, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * Output to the file descriptor. Similar to POSIX dprintf() call.
 * @see yvbprintf
 */
YYIO_nn(2, 4)
int yπvdprintf(int fd, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va);
/**
 * @}
 */

/**
 * @defgroup YYΩIO_printf TMODEN YYΩIO_printf
 * @ingroup yπio
 * @brief Printing macro callbacks.
 * @see yvbprintf
 * @{
 */
YYIO_nn(1, 2)
int YYΩIO_ybprintf(YYΩIO_printcb_t *out, void *arg, const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(1)
int YYΩIO_yprintf(const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(1, 2)
int YYΩIO_yfprintf(FILE *file, const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(1, 3) YYIO_access_w(1)
int YYΩIO_ysprintf(TCHAR *dest, size_t size, const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(1, 2)
int YYΩIO_yaprintf(TCHAR **strp, const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(1, 2)
int YYΩIO_yreaprintf(TCHAR **strp, const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(1) YYIO_retmalloc YYIO_wur
TCHAR *YYΩIO_yformatf(const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(2) YYIO_wur
TCHAR *YYΩIO_yreformatf(TCHAR *str, const yπio_printdata_t *data, const TCHAR *fmt, ...);
YYIO_nn(2)
int YYΩIO_ydprintf(int fd, const yπio_printdata_t *data, const TCHAR *fmt, ...);
/**
 * @}
 */

/**
 * @defgroup yπ__printf TMODEN yπ__printf
 * @ingroup yπio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define yπbprintf(cb, arg, ...)     YYΩIO_ybprintf(cb, arg, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπprintf(...)               YYΩIO_yprintf(YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπfprintf(file, ...)        YYΩIO_yfprintf(file, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπsprintf(dest, size, ...)  YYΩIO_ysprintf(dest, size, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπaprintf(strp, ...)        YYΩIO_yaprintf(strp, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπreaprintf(strp, ...)      YYΩIO_yreaprintf(strp, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπformatf(...)              YYΩIO_yformatf(YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπreformatf(str, ...)       YYΩIO_yreformatf(str, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπdprintf(fd, ...)          YYΩIO_ydprintf(fd, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup yπ__print TMODEN yπ__print
 * @ingroup yπio
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define yπbprint(cb, arg, ...)     YYΩIO_ybprintf(cb, arg, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπprint(...)               YYΩIO_yprintf(YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπfprint(file, ...)        YYΩIO_yfprintf(file, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπsprint(dest, size, ...)  YYΩIO_ysprintf(dest, size, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπaprint(strp, ...)        YYΩIO_yaprintf(strp, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπreaprint(strp, ...)      YYΩIO_yreaprintf(strp, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπformat(...)              YYΩIO_yformatf(YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπreformat(str, ...)       YYΩIO_yreformatf(str, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπdprint(fd, ...)          YYΩIO_ydprintf(fd, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/**
 * @def YΩIO_PRINT_ARGUMENTS
 * @ingroup yπio
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
#define YΩIO_PRINT_ARGUMENTS(...)  \
		YYIO_print_arguments_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})( \
				YYΩIO_PRINT_FUNC_GENERIC, yπio_printdata_t, __VA_ARGS__)

#endif /* YYIO_YIO_YΩIO_IO_H_ */
