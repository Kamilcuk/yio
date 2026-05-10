/**
 * @file
 * @date: 2020-lip-02
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _POSIX_C_SOURCE  1
#include "private.h"
#include "yio/private/yio_string.h"
#include <time.h>
#if (YYIO_HAS_timespec || YYIO_HAS_timeval) && YYIO_HAS_SYS_TIME_H
#include <sys/time.h>
#endif

#if YYIO_HAS_timespec || YYIO_HAS_timeval
static inline
int YYIO_print_timespec_val(yio_printctx_t *t, long long sec, long long nsec, int precision_default) {
	struct yio_printfmt_s *const pf = &t->pf;
	const char type = pf->type;
	const int precision = (int)yio_precision_get_default(pf->precision, (size_t)precision_default);

	long long max_nsec = 1;
	for (int i = 0; i < precision_default; ++i) max_nsec *= 10;

	sec += nsec / max_nsec;
	nsec %= max_nsec;
	if (nsec < 0) {
		nsec += max_nsec;
		sec--;
	}

	const bool is_neg = sec < 0;
	const unsigned long long abs_sec = is_neg ? (unsigned long long)-(sec + (nsec != 0)) : (unsigned long long)sec;
	const unsigned long long abs_nsec = is_neg ? (nsec == 0 ? 0 : (unsigned long long)(max_nsec - nsec)) : (unsigned long long)nsec;

	YYIO_string res;
	YYIO_string_init(&res);

	int err = 0;
	unsigned long long fraction = abs_nsec;
	if (precision < precision_default) {
		unsigned long long divisor = 1;
		for (int i = 0; i < precision_default - precision; ++i) divisor *= 10;
		fraction /= divisor;
	} else if (precision > precision_default) {
		for (int i = 0; i < precision - precision_default; ++i) fraction *= 10;
	}

	if (is_neg) {
		err = YYIO_string_putc(&res, '-');
		if (err) goto end;
	}

	const bool dot_added = precision > 0 || pf->hash;
	if (type == 'f' || type == 'g') {
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){0}, abs_sec);
		if (err) goto end;
		if (dot_added) {
			err = YYIO_string_putc(&res, '.');
			if (err) goto end;
			if (precision > 0) {
				err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = (uint16_t)precision + 1, .align = '=', .fill = '0'}, fraction);
				if (err) goto end;
			}
		}
	} else if (type == '\0') {
		const unsigned long long h = abs_sec / 3600;
		const unsigned long long m = (abs_sec % 3600) / 60;
		const unsigned long long s = abs_sec % 60;
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){0}, h);
		if (err) goto end;
		err = YYIO_string_putc(&res, ':');
		if (err) goto end;
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = 3, .align = '=', .fill = '0'}, m);
		if (err) goto end;
		err = YYIO_string_putc(&res, ':');
		if (err) goto end;
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = 3, .align = '=', .fill = '0'}, s);
		if (err) goto end;
		if (dot_added) {
			err = YYIO_string_putc(&res, '.');
			if (err) goto end;
			if (precision > 0) {
				err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = (uint16_t)precision + 1, .align = '=', .fill = '0'}, fraction);
				if (err) goto end;
			}
		}
	} else {
		err = YIO_ERROR_INVALID_TYPE;
		goto end;
	}

	if ((type == 'g' || (type == '\0' && pf->hash)) && dot_added) {
		YYIO_string_remove_trailing_zeros_and_dot(&res);
	}
	err = yio_printctx_put(t, YYIO_string_data(&res), YYIO_string_len(&res));

end:
	YYIO_string_fini(&res);
	return err;
}
#endif

#if YYIO_HAS_timespec
int YYIO_print_timespec(yio_printctx_t *t) {
	const struct timespec ts = yio_printctx_va_arg(t, struct timespec);
	int err = yio_printctx_init(t);
	if (err) return err;
	return YYIO_print_timespec_val(t, (long long)ts.tv_sec, (long long)ts.tv_nsec, 9);
}
#endif // YYIO_HAS_timespec

#if YYIO_HAS_timeval
int YYIO_print_timeval(yio_printctx_t *t) {
	const struct timeval ts = yio_printctx_va_arg(t, struct timeval);
	int err = yio_printctx_init(t);
	if (err) return err;
	return YYIO_print_timespec_val(t, (long long)ts.tv_sec, (long long)ts.tv_usec, 6);
}
#endif // YYIO_HAS_timeval
