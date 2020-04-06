/**
 * @file yio.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio/yio_public.h"
#include <stdio.h>
#include <stdarg.h>

/* ------------------------------------------------------------------------- */

/**
 * Print the arguments using a custom callback function.
 * Actually we could name it: "register" function to print those arguments.
 * void yiocb(int callback(yio_scanctx_t *), ...);
 * void yiocb(int callback(yio_printctx_t *), ...);
 * @param callback Callback to call. Must be yio_printcb_t or yio_scancb_t type.
 * @param ... Arguments to be passed to contexts va_list.
 */
#define yiocb(callback, ...)  ((callback, ##__VA_ARGS__),_yIO_61COMMAS)

/* Exported Print Symbols --------------------------------------------------- */

/**
 * @defgroup yp__mod Printing modifiers.
 * @{
 **/
/// Align output to the right.
#define ypright()         yiocb(_yIO_print_right)
/// Align output to the left.
#define ypleft()          yiocb(_yIO_print_left)
/// Center the output within width.
#define ypcenter()        yiocb(_yIO_print_center)
/// Outputs the next float in fixed format.
#define ypfixed()         yiocb(_yIO_print_fixed)
/// Outputs the next float in scientific format.
#define ypscientific()    yiocb(_yIO_print_scientific)
/// Print floats using hexadecimal notation.
#define yphexfloat()      yiocb(_yIO_print_hexfloat)
/// Set formatting precision.
#define ypprecision(arg)  yiocb(_yIO_print_precision, _yIO_CAST_TO_INT(arg))
/// Set field width.
#define ypwidth(arg)      yiocb(_yIO_print_width, _yIO_CAST_TO_INT(arg))
/// Print the unit.
#define ypunitbuf()       yiocb(_yIO_print_unitbuf)
/// Do not print the unit.
#define ypnounitbuf()     yiocb(_yIO_print_nounitbuf)
/**
 * Format output by reading python format string.
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 */
#define yppfmt(...)  \
		yiocb(_yIO_print_pfmt, _yIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))
/**
 * Format output by reading C format string.
 * @param ... C formatting string like "%s".
 * @param ... Optional up to two int values.
 */
#define ypcfmt(...)  \
		yiocb(_yIO_print_cfmt, _yIO_ypcfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup yp__ Printing overloads.
 * @{
 */
/// Print wchar_t character.
#define ypwchar(wchar)         yiocb(_yIO_print_wchar, wchar)
/// Print a string of wchar_t characters.
#define ypwstrint(wstring)       yiocb(_yIO_print_wstring, wstring)
/**
 * @}
 */

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
int yvbprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, va_list *va);
/**
 * Similar to printf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yvprintf(yio_printdata_t *data, va_list *va);
/**
 * Similar to fprintf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yvfprintf(FILE *file, yio_printdata_t *data, va_list *va);
/**
 * Similar to snprintf() call.
 * @see yvbprintf
 */
_yIO_nn()
int yvsprintf(char *dest, size_t size, yio_printdata_t *data, va_list *va);
/**
 * Works as-if the call to GNU extension asprintf().
 * Equal to doing: *strp = NULL; yvreasrintf(strp, ...);
 * @see yvreasprintf
 */
_yIO_nn()
int yvaprintf(char **strp, yio_printdata_t *data, va_list *va);
/**
 * Reallocates a string to append the printed characters.
 * @param strp If is NULL, a new string is allocated, otherwise it is reallocated.
 * @param data
 * @param va
 * @return Number of characters written or negative on error.
 *         In case of memory error the pointer strp is freed and is set to NULL.
 */
_yIO_nn()
int yvreaprintf(char **strp, yio_printdata_t *data, va_list *va);
/**
 * Calls yvreformatf(NULL, ...). See yvreformatf.
 * @see yvreformatf
 */
_yIO_malloc _yIO_nn() _yIO_wur
char *yvformatf(yio_printdata_t *data, va_list *va);
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
char *yvreformatf(char *str, yio_printdata_t *data, va_list *va);
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
#define ybprintf(cb, arg, ...)     _yIO_ybprintf(cb, arg, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yprintf(...)               _yIO_yprintf(YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yfprintf(file, ...)        _yIO_yfprintf(file, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define ysprintf(dest, size, ...)  _yIO_ysprintf(dest, size, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yaprintf(strp, ...)        _yIO_yaprintf(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yreaprintf(strp, ...)      _yIO_yreaprintf(strp, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yformatf(...)              _yIO_yformatf(YIO_PRINT_ARGUMENTS(__VA_ARGS__))
#define yreformatf(str, ...)       _yIO_yreformatf(str, YIO_PRINT_ARGUMENTS(__VA_ARGS__))
/**
 * @}
 */

/**
 * @defgroup y__print Print stream-like functions.
 * @{
 * @}
 * @ingroup y__printf
 * @brief Constructs yio printing data and calls internal Yio operation function.
 * @see yvbprintf
 * @{
 */
#define ybprint(cb, arg, ...)     _yIO_ybprintf(cb, arg, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yprint(...)               _yIO_yprintf(YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yfprint(file, ...)        _yIO_yfprintf(file, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define ysprint(dest, size, ...)  _yIO_ysprintf(dest, size, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yaprint(strp, ...)        _yIO_yaprintf(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yreaprint(strp, ...)      _yIO_yreaprintf(strp, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yformat(...)              _yIO_yformatf(YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
#define yreformat(str, ...)       _yIO_yreformatf(str, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**
 * @}
 */

/* Exported Print Symbols --------------------------------------------------- */

/**
 * @defgroup yscan Scanners.
 * @{
 */
/**
 * Scan until they are the characters in \c fmt string.
 * @param fmt string List of characters to scan until.
 * @param dest char* or char** Pointer to destination character.
 *             It may be a pointer to static buffer or characters,
 *             Or a pointer to pointer to char to use dynamic allocation.
 */
#define yscan_until(fmt, dest)   yiocb( \
		_Generic((dest), \
				char *: _yIO_scan_until, \
				char **:  _yIO_scan_until_charpntpnt \
		), \
		_Generic((fmt), char *: (const char *)(fmt), const char *: (fmt)), \
		sizeof(dest), \
		_Generic((dest),char *: (dest), char(*)[sizeof(*dest)]: (dest), char **: (dest)) \
)
/**
 * Scan all characters until they are not the character in \c fmt string.
 * @param fmt List of characters to exclude.
 * @param dest Pointer to destination string.
 *             It may be a pointer to static buffer or characters,
 *             Or a pointer to pointer to char to use dynamic allocation.
 */
#define yscan_except(fmt, dest)  yiocb( \
		_Generic((dest), \
				char *: _yIO_scan_except, \
				char **: _yIO_scan_except_charpntpnt \
		), \
		_Generic((fmt), char *: (const char *)(fmt), const char *: (fmt)), \
		sizeof(dest), \
		_Generic((dest),char *: (dest), char(*)[sizeof(*dest)]: (dest), char **: (dest)) \
)
/**
 * @}
 */

/**
 * @defgroup ys_mod Scanning modifiers.
 * @{
 */
/// Ignore next input.
#define ysignore()         yiocb(_yIO_scan_ignore)
/// Set the maximum field width. Negative disables it.
#define yswidth(width)     yiocb(_Generic((width), int: _yIO_scan_width_int, size_t: _yIO_scan_width_size_t), width)
/**
 * @}
 */

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
struct yio_scanret_s yvbscanf(_yIO_scancb_t *in, void *arg, yio_scandata_t *data, va_list *va);
/// Scans from standard input.
_yIO_nn() _yIO_wur
struct yio_scanret_s yvscanf(yio_scandata_t *data, va_list *va);
/// Scans from FILE*.
_yIO_nn() _yIO_wur
struct yio_scanret_s yvfscanf(FILE *file, yio_scandata_t *data, va_list *va);
/// Scans from a string \b str .
_yIO_nn() _yIO_wur
struct yio_scanret_s yvsscanf(char *src, yio_scandata_t *data, va_list *va);
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
 * @}
 */


