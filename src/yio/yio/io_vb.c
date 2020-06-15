/**
 * @file
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config();
#include "private.h"

/* yvbprintf and yvbscanf helpers ------------------------------------------------------ */

static inline _yIO_wur _yIO_nn() _yIO_rnn
const Ychar *_yIO_strpbrk_braces(const Ychar *str) {
	while (*str) {
		if (*str == Yc('{') || *str == Yc('}')) {
			break;
		}
		++str;
	}
	return str;
}

static inline
int _yIO_yvbgeneric_iterate_until_format(const Ychar *fmt, const Ychar **endptr,
		int (*callback)(void *arg, const Ychar *begin, const Ychar *end), void *arg) {
	bool double_braces_found = false;
	while (fmt[0]) {
		const Ychar * const after_brace = fmt + double_braces_found;
		const Ychar * const brace = _yIO_strpbrk_braces(after_brace);
		if (after_brace != brace) {
			const int err = callback(arg, fmt, brace);
			if (err) return err;
			fmt = brace;

			if (fmt[0] == Yc('\0')) {
				break;
			}
		}
		fmt = brace;

		assert(fmt[0] == Yc('{') || fmt[0] == Yc('}'));
		double_braces_found = fmt[0] == fmt[1];
		if (double_braces_found) {
			++fmt;
			continue;
		}
		break;
	}
	*endptr = fmt;
	return 0;
}

static inline
int yvbprintf_iterate_until_format_callback(void *arg, const Ychar *begin, const Ychar *end) {
	yio_printctx_t *t = arg;
	return yio_printctx_out(t, begin, end - begin);
}

/* yvbprintf ----------------------------------------------------------- */

static inline
int yvbprintf_in(yio_printctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->pf = _yIO_printfmt_default;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}

	while (1) {
		int err = _yIO_yvbgeneric_iterate_until_format(t->fmt, &t->fmt,
				yvbprintf_iterate_until_format_callback, t);
		if (err) return err;
		if (t->fmt[0] == Yc('\0')) break;

		err = _yIO_pfmt_parse(t, &t->pf, t->fmt, &t->fmt);
		if (err) return err;

		if (t->ifunc == NULL || *t->ifunc == NULL) {
			return YIO_ERROR_TOO_MANY_FMT;
		}
		const int ifuncret = (*t->ifunc)(t);
		if (ifuncret) {
			return ifuncret;
		}
		t->ifunc++;

	}
	return 0;
}

int yvbprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, va_list *va) {
	assert(out != NULL);
	assert(data != NULL);
	assert(va != NULL);
	yio_printctx_t _ctx = _yIO_printctx_init(out, arg, data, va);
	yio_printctx_t * const t = &_ctx;
	const int err = yvbprintf_in(t);
	if (err) {
		return -abs(err);
	}
	if (t->writtencnt > INT_MAX) {
		return INT_MAX;
	}
	return t->writtencnt;
}

/* yvbscan ----------------------------------------------------------------------------- */

static inline
int _yIO_yvbscanf_iterate_until_format_callback(void *arg, const char *begin, const char *end) {
	yio_scanctx_t *t = arg;
	return _yIO_scan_string_literal_in(t, begin, end);
}

static inline
int _yIO_yvbscanf_in(yio_scanctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->sf = _yIO_scanfmt_default;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}

	while (1) {
		int err = _yIO_yvbgeneric_iterate_until_format(t->fmt, &t->fmt,
				_yIO_yvbscanf_iterate_until_format_callback, t);
		if (err) return err;
		if (t->fmt[0] == Yc('\0')) break;

		err = _yIO_scan_parse_scanfmt(t, &t->sf, t->fmt, &t->fmt);
		if (err) return err;

		if (t->sf.ignore && t->sf.type) {
			// not implemented yet
			return YIO_ERROR_ENOSYS;
		} else {
			if (t->ifunc == NULL || *t->ifunc == NULL) {
				return YIO_ERROR_TOO_MANY_FMT;
			}
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
			t->ifunc++;
		}
	}
	return 0;
}

struct yio_scanret_s yvbscanf(_yIO_scancb_t *in, void *arg,
		yio_scandata_t *data,
		va_list *va) {
	yio_scanctx_t _ctx = _yIO_scanctx_init(in, arg, data, va);
	yio_scanctx_t * const t = &_ctx;
	const int err = _yIO_yvbscanf_in(t);
	const struct yio_scanret_s scanret = {
			.error = err,
			.count = t->scannedcnt,
			.nitem = t->ifunc - data->funcs,
			.lastchar = t->ch,
	};
	return scanret;
}
