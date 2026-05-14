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

#ifdef __SDCC
#define YYIO_PRINT_GENERIC_TM(X, XALIAS)
#else

int YYIO_print_tm(yio_printctx_t *t);
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

#define YYIO_PRINT_GENERIC_TM(X, XALIAS) \
		X(struct tm, YYIO_print_tm) \
		YYIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, struct tm, YYIO_print_tm_pointer)

#endif //__SDCC

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_TM_H_
