/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config_yio() m4_dnl;
#include "private.h"

int _yIO_scan_string(yio_scanctx_t *t) {
	char * str = yio_scanctx_va_arg(t, char *);
	const size_t strsize = yio_scanctx_arg_size_next(t);
	const struct yio_scanfmt_s *f = yio_scanctx_get_fmt(t);
	const size_t width = f->width < 0 ? INT_MAX : f->width;
	int err = 0;
	char * const str0 = str;
	int count = ((strsize - 1) < width) ? strsize - 1 : width;
	for (int c = 0; (err = yio_scanctx_in(t, &c)) == 0 && c != EOF; ) {
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
		yio_scanctx_unin(t);
	}
	if (str0 == str) {
		err = YIO_ERROR_SCAN_NOTHING;
	}
	return err;
}

int _yIO_scan_string_literal_in(yio_scanctx_t *t, const char *str, const char *end) {
	for (; str != end; str++) {
		int c;
		const int err = yio_scanctx_in(t, &c);
		// _yIO_dbgln("11 err=%d fmt[0]=%s c=%s", err, _yIO_printc(fmt[0]), _yIO_printc(c));
		if (err) return err;
		if (str[0] != c) {
			return YIO_ERROR_SCAN_NOT_MATCHED;
		}
	}
	return 0;
}

int _yIO_scan_string_literal(yio_scanctx_t *t) {
	const char * str = yio_scanctx_va_arg(t, char *);
	return _yIO_scan_string_literal_in(t, str, str + strlen(str));
}

int _yIO_scan_except(yio_scanctx_t *t) {
	const char *until = yio_scanctx_va_arg(t, const char *);
	size_t destsize = yio_scanctx_va_arg(t, size_t);
	char *str = yio_scanctx_va_arg(t, char *);
	int err = 0;
	for (int c = 0; (err = yio_scanctx_in(t, &c)) == 0 && c != EOF; ) {
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
	yio_scanctx_unin(t);
	if (err == 0 && destsize == 1) {
		return YIO_ERROR_ENOMEM;
	}
	return err;
}

int _yIO_scan_except_charpntpnt(yio_scanctx_t *t) {
	const char * const until = yio_scanctx_va_arg(t, const char *);
	const size_t ignored = yio_scanctx_va_arg(t, size_t);
	(void)ignored;
	assert(ignored == sizeof(char*));
	char ** const dest = yio_scanctx_va_arg(t, char **);
	char *str = NULL;
	int err;
	for (int c = 0; (err = yio_scanctx_in(t, &c)) == 0 && c != EOF; ) {
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
