/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include "scan_string.h"

int _yΩIO_scan_string(yπio_scanctx_t *t) {
	char * str = yπio_scanctx_va_arg(t, char *);
	const size_t strsize = yπio_scanctx_arg_size_next(t);
	const struct yπio_scanfmt_s *f = yπio_scanctx_get_fmt(t);
	const size_t width = f->width < 0 ? INT_MAX : f->width;
	int err = 0;
	char * const str0 = str;
	int count = ((strsize - 1) < width) ? strsize - 1 : width;
	for (Yint c = 0; (err = yπio_scanctx_in(t, &c)) == 0 && c != YEOF; ) {
		if (c == ' ' || c == '\t' || c == '\n') {
			break;
		}
		*str = c;
		str++;
		count--;
		if (count == 0) {
			break;
		}
	}
	if (strsize) {
		*str = '\0';
	}
	if (count) {
		yπio_scanctx_unin(t);
	}
	if (str0 == str) {
		err = YIO_ERROR_SCAN_NOTHING;
	}
	return err;
}

int _yΩIO_scan_string_literal_in(yπio_scanctx_t *t, const Ychar *str, const Ychar *end) {
	for (; str != end; str++) {
		Yint c;
		const int err = yπio_scanctx_in(t, &c);
		// _yIO_dbgln("11 err=%d fmt[0]=%s c=%s", err, _yIO_printc(fmt[0]), _yIO_printc(c));
		if (err) return err;
		if ((Yint)str[0] != c) {
			return YIO_ERROR_SCAN_NOT_MATCHED;
		}
	}
	return 0;
}

int _yΩIO_scan_string_literal(yπio_scanctx_t *t) {
	const char * str = yπio_scanctx_va_arg(t, char *);
	return _yΩIO_scan_string_literal_in(t, (void*)str, (void*)str + strlen(str));
}

int _yΩIO_scan_except(yπio_scanctx_t *t) {
	const char *until = yπio_scanctx_va_arg(t, const char *);
	size_t destsize = yπio_scanctx_va_arg(t, size_t);
	char *str = yπio_scanctx_va_arg(t, char *);
	int err = 0;
	for (Yint c = 0; (err = yπio_scanctx_in(t, &c)) == 0 && c != YEOF; ) {
		if (strchr(until, c) != NULL) {
			break;
		}
		if (destsize <= 1) {
			break;
		}
		*str = c;
		str++;
		destsize--;
	}
	*str = '\0';
	yπio_scanctx_unin(t);
	if (err == 0 && destsize == 1) {
		return YIO_ERROR_ENOMEM;
	}
	return err;
}

int _yΩIO_scan_except_charpntpnt(yπio_scanctx_t *t) {
	const char * const until = yπio_scanctx_va_arg(t, const char *);
	const size_t ignored = yπio_scanctx_va_arg(t, size_t);
	(void)ignored;
	assert(ignored == sizeof(char*));
	char ** const dest = yπio_scanctx_va_arg(t, char **);
	char *str = NULL;
	int err;
	for (Yint c = 0; (err = yπio_scanctx_in(t, &c)) == 0 && c != YEOF; ) {
		if (strchr(until, c) != NULL) {
			break;
		}
		str = yreformat(str, (char)c);
		if (str == NULL) {
			err = YIO_ERROR_ENOMEM;
			break;
		}
	}
	*dest = str;
	return err;
}
