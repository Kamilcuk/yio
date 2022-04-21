/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "ctx.h"
#include <stdio.h>
#include <stdarg.h>

/* Exported Print Symbols --------------------------------------------------- */

/**
 * @defgroup yv__print Printing functions with va_list.
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
_yIO_nn(1, 3, 5)
int yπvbprintf(_yΩIO_printcb_t *out, void *arg, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Similar to printf() call.
 * @see yvbprintf
 */
_yIO_nn(1, 3)
int yπvprintf(const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yvbprintf
 */
_yIO_nn(1, 2, 4) _yIO_access_r(1) _yIO_access_r(2) _yIO_access_r(3) _yIO_access_r(4)
int yπvfprintf(FILE *file, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Similar to snprintf() call.
 * @see yvbprintf
 */
_yIO_nn(1, 3, 5) _yIO_access_w(1) _yIO_access_r(3) _yIO_access_r(4) _yIO_access_r(5)
int yπvsprintf(Ychar *dest, size_t size, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Works as-if the call to GNU extension asprintf().
 * Equal to doing: *strp = NULL; yvreasrintf(strp, ...);
 * @see yvreasprintf
 */
_yIO_nn(1, 2, 4)
int yπvaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Reallocates a string to append the printed characters.
 * @param strp If is NULL, a new string is allocated, otherwise it is reallocated.
 * @param data
 * @param va
 * @return Number of characters written or negative on error.
 *         In case of memory error the pointer strp is freed and is set to NULL.
 */
_yIO_nn(1, 2, 4)
int yπvreaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Calls yπvreformatf(NULL, ...). See yπvreformatf.
 * @see yπvreformatf
 */
_yIO_malloc _yIO_nn(1, 3) _yIO_wur
Ychar *yπvformatf(const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
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
_yIO_nn(2, 4) _yIO_wur
Ychar *yπvreformatf(Ychar *str, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * Output to the file descriptor. Similar to POSIX dprintf() call.
 * @see yvbprintf
 */
_yIO_nn(2, 4)
int yπvdprintf(int fd, const yπio_printdata_t *data, const Ychar *fmt, va_list *va);
/**
 * @}
 */

/**
 * @defgroup y__yIO_printf Printing macro callbacks.
 * @{
 * @}
 * @ingroup y__yIO_printf
 * @brief
 * @see yvbprintf
 * @{
 */
_yIO_nn(1, 2)
int _yΩIO_ybprintf(_yΩIO_printcb_t *out, void *arg, const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(1)
int _yΩIO_yprintf(const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(1, 2)
int _yΩIO_yfprintf(FILE *file, const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(1, 3) _yIO_access_w(1)
int _yΩIO_ysprintf(Ychar *dest, size_t size, const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(1, 2)
int _yΩIO_yaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(1, 2)
int _yΩIO_yreaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(1) _yIO_malloc _yIO_wur
Ychar *_yΩIO_yformatf(const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(2) _yIO_wur
Ychar *_yΩIO_yreformatf(Ychar *str, const yπio_printdata_t *data, const Ychar *fmt, ...);
_yIO_nn(2)
int _yΩIO_ydprintf(int fd, const yπio_printdata_t *data, const Ychar *fmt, ...);
/**
 * @}
 */

/**
 * @defgroup y__printf Print formatted output.
 * @{
 * @}
 * @ingroup y__printf
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define yπbprintf(cb, arg, ...)     _yΩIO_ybprintf(cb, arg, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπprintf(...)               _yΩIO_yprintf(YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπfprintf(file, ...)        _yΩIO_yfprintf(file, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπsprintf(dest, size, ...)  _yΩIO_ysprintf(dest, size, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπaprintf(strp, ...)        _yΩIO_yaprintf(strp, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπreaprintf(strp, ...)      _yΩIO_yreaprintf(strp, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπformatf(...)              _yΩIO_yformatf(YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπreformatf(str, ...)       _yΩIO_yreformatf(str, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yπdprintf(fd, ...)          _yΩIO_ydprintf(fd, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup y__print Print stream-like functions.
 * @{
 * @}
 * @ingroup y__print
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define yπbprint(cb, arg, ...)     _yΩIO_ybprintf(cb, arg, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπprint(...)               _yΩIO_yprintf(YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπfprint(file, ...)        _yΩIO_yfprintf(file, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπsprint(dest, size, ...)  _yΩIO_ysprintf(dest, size, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπaprint(strp, ...)        _yΩIO_yaprintf(strp, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπreaprint(strp, ...)      _yΩIO_yreaprintf(strp, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπformat(...)              _yΩIO_yformatf(YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπreformat(str, ...)       _yΩIO_yreformatf(str, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπdprint(fd, ...)          _yΩIO_ydprintf(fd, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/**
 * YΩIO_PRINT_ARGUMENTS
 * Create argument list for yprint functions
 * If an argument doesn't has 62 commas, then the function for it is chosen
 *    using _Generic expression with _yIO_PRINT_FUNC_GENERIC.
 * If an argument does has 62 commas, then the first argument that should be inside braces is extracted
 *   and the first argument from inside the braces is the function to handle the arguments.
 * The arguments are appended to the end of the function.
 *
 * For example a call like:
 *    yprint(1, yiocb(function, 2, 3));
 * Is expanded to:
 *    yprint((const _yIO_func_t[]){ _Generic((1), int: _yIO_print_int), function, NULL }, 1, 2, 3);
 *
 * The first argument is an array of functions to handle arguments, delimitered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define YΩIO_PRINT_ARGUMENTS(...)  \
		_yΩIO_print_arguments_N(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})(_yΩIO_PRINT_FUNC_GENERIC, __VA_ARGS__)

