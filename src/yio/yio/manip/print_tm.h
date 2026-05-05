/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_TM_H_
#define YYIO_YIO_YIO_MANIP_PRINT_TM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"
#include <time.h>

/**
 * Copies the formatting string that will be passed to strftime
 * in such way that \{\{ and \}\} sequences are transformed into \{ and \}.
 * A freestanding } ends the formatting string.
 * A freestanding { is a formatting error.
 * @param dest Either a NULL or a valid pointer to allocated memory enough
 * to hold the result.
 * @param fmt
 * @return Negative on error. Otherwise returns the number of bytes excluding
 * zero terminating character that would have been written to or were written to
 * @c dest depending if it's a null pointer or not.
 */
int YYIO_print_time_in_extract_format_add_space(char *dest, const char *fmt, const char **enptr);

#ifndef __SDCC
int YYIO_print_tm(yio_printctx_t *t);
#endif
int YYIO_print_tm_pointer(yio_printctx_t *t);
int YYIO_print_localtime(yio_printctx_t *t);
int YYIO_print_gmtime(yio_printctx_t *t);

/**
 * @define yio_localtime(v)
 * @param v time_t value
 * Calls localtime() on v and prints it.
 */
#define yio_localtime(v)  yio_callback(YYIO_print_localtime, (time_t)(v))
/**
 * @define yio_gmtime(v)
 * @param v time_t value
 * Calls gmtime() on v and prints it.
 */
#define yio_gmtime(v)     yio_callback(YYIO_print_gmtime, (time_t)(v))

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_TM_H_
