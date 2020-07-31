/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio_template(`m4_dnl);
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
_yIO_nn(1, 3, 4)
int yπvbprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, va_list *va);
/**
 * Similar to printf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yπvprintf(yio_printdata_t *data, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yπvfprintf(FILE *file, yio_printdata_t *data, va_list *va);
/**
 * Similar to snprintf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yπvsprintf(Ychar *dest, size_t size, yio_printdata_t *data, va_list *va);
/**
 * Works as-if the call to GNU extension asprintf().
 * Equal to doing: *strp = NULL; yvreasrintf(strp, ...);
 * @see yvreasprintf
 */
_yIO_nn()
int yπvaprintf(Ychar **strp, yio_printdata_t *data, va_list *va);
/**
 * Reallocates a string to append the printed characters.
 * @param strp If is NULL, a new string is allocated, otherwise it is reallocated.
 * @param data
 * @param va
 * @return Number of characters written or negative on error.
 *         In case of memory error the pointer strp is freed and is set to NULL.
 */
_yIO_nn()
int yπvreaprintf(Ychar **strp, yio_printdata_t *data, va_list *va);
/**
 * Calls yvreformatf(NULL, ...). See yvreformatf.
 * @see yvreformatf
 */
_yIO_malloc _yIO_nn() _yIO_wur
Ychar *yπvformatf(yio_printdata_t *data, va_list *va);
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
_yIO_nn(2, 3) _yIO_wur
Ychar *yπvreformatf(Ychar *str, yio_printdata_t *data, va_list *va);
/**
 * Output to the file descriptor. Similar to POSIX dprintf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yπvdprintf(int fd, yio_printdata_t *data, va_list *va);
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
int _yΩIO_ybprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, ...);
_yIO_nn(1)
int _yΩIO_yprintf(yio_printdata_t *data, ...);
_yIO_nn(1, 2)
int _yΩIO_yfprintf(FILE *file, yio_printdata_t *data, ...);
_yIO_nn(1, 3)
int _yΩIO_ysprintf(Ychar *dest, size_t size, yio_printdata_t *data, ...);
_yIO_nn(1, 2)
int _yΩIO_yaprintf(Ychar **strp, yio_printdata_t *data, ...);
_yIO_nn(1, 2)
int _yΩIO_yreaprintf(Ychar **strp, yio_printdata_t *data, ...);
_yIO_nn(1) _yIO_malloc _yIO_wur
Ychar *_yΩIO_yformatf(yio_printdata_t *data, ...);
_yIO_nn(1, 2) _yIO_wur
Ychar *_yΩIO_yreformatf(Ychar *str, yio_printdata_t *data, ...);
_yIO_nn(2)
int _yΩIO_ydprintf(int fd, yio_printdata_t *data, ...);
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
#define yπbprint(cb, arg, ...)     _yIO_ybprintf(cb, arg, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπprint(...)               _yIO_yprintf(YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπfprint(file, ...)        _yIO_yfprintf(file, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπsprint(dest, size, ...)  _yIO_ysprintf(dest, size, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπaprint(strp, ...)        _yIO_yaprintf(strp, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπreaprint(strp, ...)      _yIO_yreaprintf(strp, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπformat(...)              _yIO_yformatf(YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπreformat(str, ...)       _yIO_yreformatf(str, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yπdprint(fd, ...)          _yIO_ydprintf(fd, YΩIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/* Exported Scan Symbols --------------------------------------------------- */

/**
 * Scanning result.
 * All y*scan(f)? functions return this.
 */
struct yπio_scanret_s {
	/**
	 * YIO_ERROR_* number or 0 on success.
	 */
	int error;
	/**
	 * Count of items successfully scanned.
	 */
	unsigned nitem;
	/**
	 * Count of characters that were read,
	 * including the lastchar.
	 */
	size_t count;
	/**
	 * Last character that was read.
	 * When all input was read, this will be EOF.
	 */
	int lastchar;
};

/**
 * @defgroup yv__scanf Scanning functions with va_list.
 * Scanning functions with va_list.
 * @{
 */
/**
 * Scans data using yio scanning data.
 * @param in A custom callback to read characters from.
 * @param arg A context passed to the callback.
 * @param data Pointer to scanning data.
 * @param va A pointer to va_list.
 * @return struct yio_scanret_s
 */
_yIO_nn(1, 3, 4) _yIO_wur
struct yπio_scanret_s yπvbscanf(_yIO_scancb_t *in, void *arg, yio_scandata_t *data, va_list *va);
/// Scans from standard input.
_yIO_nn() _yIO_wur
struct yπio_scanret_s yπvscanf(yio_scandata_t *data, va_list *va);
/// Scans from FILE*.
_yIO_nn() _yIO_wur
struct yπio_scanret_s yπvfscanf(FILE *file, yio_scandata_t *data, va_list *va);
/// Scans from a string \b str .
_yIO_nn() _yIO_wur
struct yπio_scanret_s yπvsscanf(Ychar *src, yio_scandata_t *data, va_list *va);
/// Scans from a file descriptor.
_yIO_nn() _yIO_wur
struct yπio_scanret_s yπvdscanf(int fd, yio_scandata_t *data, va_list *va);
/**
 * @}
 */

/**
 * @defgroup y__scanf Formatted scanning function.
 * @{
 * @}
 * @ingroup y__scanf
 * @brief Constructs Yio scanning data and calls internal Yio operation function.
 * @see yvbscanf
 * @return struct yio_scanret_s
 * @{
 */
#define ybscanf(cb, arg, ...)  _yIO_ybscanf(cb, arg, YIO_SCAN_ARGUMENTS(__VA_ARGS__))
#define yscanf(...)            _yIO_yscanf(YIO_SCAN_ARGUMENTS(__VA_ARGS__))
#define yfscanf(file, ...)     _yIO_yfscanf(file, YIO_SCAN_ARGUMENTS(__VA_ARGS__))
#define ysscanf(src, ...)      _yIO_ysscanf(src, YIO_SCAN_ARGUMENTS(__VA_ARGS__))
#define ydscanf(fd, ...)       _yIO_yscanf(fd, YIO_SCAN_ARGUMENTS(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup y__yIO_scan Scanning macro callbacks.
 * @{
 * @}
 * @ingroup y__yIO_scan
 * @brief
 * @see yvbscanf
 * @{
 */
_yIO_nn(1, 3) _yIO_wur
struct yio_scanret_s _yIO_ybscanf(_yIO_scancb_t *in, void *arg, yio_scandata_t *data, ...);
_yIO_nn(1) _yIO_wur
struct yio_scanret_s _yIO_yscanf(yio_scandata_t *data, ...);
_yIO_nn(1, 2) _yIO_wur
struct yio_scanret_s _yIO_yfscanf(FILE *file, yio_scandata_t *data, ...);
_yIO_nn(1, 2) _yIO_wur
struct yio_scanret_s _yIO_ysscanf(Ychar *src, yio_scandata_t *data, ...);
_yIO_nn(2) _yIO_wur
struct yio_scanret_s _yIO_ydscanf(int fd, yio_scandata_t *data, ...);
/**
 * @}
 */

/**
 * @defgroup y__scan Stream-like scanning function.
 * @{
 * @}
 * @ingroup y__scan
 * @brief Constructs Yio scanning data and calls internal Yio operation function.
 * @see yvbscanf
 * @{
 */
/**
 * Scan from standard input.
 * @param cb See yvbscanf
 * @param arg See yvbscanf
 * @param ...
 * @return struct yio_scanret_s
 */
#define ybscan(cb, arg, ...)  _yIO_ybscanf(cb, arg, YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * Scan from standard input.
 * @param ...
 * @return struct yio_scanret_s
 */
#define yscan(...)            _yIO_yscanf(YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @param file FILE* to scan from.
 * @param ...
 * @return struct yio_scanret_s
 */
#define yfscan(file, ...)     _yIO_yfscanf(file, YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @param src char* String to scan data from.
 * @param ...
 * @return struct yio_scanret_s
 */
#define ysscan(src, ...)      _yIO_ysscanf(src, YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * Scan from file descriptor.
 * @param ...
 * @return struct yio_scanret_s
 */
#define ydscan(fd, ...)       _yIO_ydscanf(fd, YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/* ---------------------------------------------------------------------------- */

/**
 * YIO_PRINT_ARGUMENTS
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
 * The first argument is an array of functions to handle arguments, delimetered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define YIO_PRINT_ARGUMENTS(...)  \
		_yIO_print_arguments_N(__VA_ARGS__, m4_seqcomma(m4_MLVLS, 0))(_yIO_PRINT_FUNC_GENERIC, __VA_ARGS__)

/**
 * @def YIO_SCAN_ARGUMENTS(...)
 * @param ... Scan arguments list.
 * Just like YIO_PRINT_ARGUMENTS.
 */
#define YIO_SCAN_ARGUMENTS(...)  \
		_yIO_scan_arguments_N(__VA_ARGS__, m4_seqcomma(m4_MLVLS, 0))(_yIO_SCAN_FUNC_GENERIC, __VA_ARGS__)


~)m4_dnl;