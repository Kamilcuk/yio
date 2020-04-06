/**
 * @file yio_fmt.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio_ctx.h"
#include <stdbool.h>

/**
 * @ingroup printfmt
 * The default values of printfmt
 */
extern const struct yio_printfmt_s _yIO_printfmt_default;

/**
 * @ingroup scanfmt
 * The default values of scanfmt
 */
extern const struct yio_scanfmt_s _yIO_scanfmt_default;

/**
 * @ingroup printfmt
 */
_yIO_nn()
void _yIO_printfmt_println(const struct yio_printfmt_s *t);

/**
 * Parse pythong formatting string
 * @param c
 * @param pf
 * @param fmt Python formatting string like "{: < -0#123_.456}"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
_yIO_wur _yIO_nn()
int _yIO_pfmt_parse(struct _yIO_commonctx_s *c, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr);

/**
 * Parse C formatting strnig
 * @param c
 * @param pf
 * @param fmt C format string like "%+- #123.456s"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
_yIO_wur _yIO_nn()
int _yIO_cfmt_parse(struct _yIO_commonctx_s *c, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr);

/**
 * @ingroup printmod
 * @param t
 * @param str The number printed as a string without leading sign.
 * @param positive Is the number positive or negative?
 * @return 0 on success, error otherwise
 */
_yIO_wur _yIO_nn()
int _yIO_print_format_number(yio_printctx_t *t, const char str[], bool positive);

/**
 * @ingroup printmod
 * @param t
 * @param str A string.
 * @return 0 on success, error otherwise
 */
_yIO_wur _yIO_nn()
int _yIO_print_format_string(yio_printctx_t *t, const char str[]);

_yIO_wur _yIO_nn(1, 2, 3)
int _yIO_scan_parse_scanfmt(struct _yIO_commonctx_s *c, struct yio_scanfmt_s *sf,
		const char *fmt, const char **endptr);

_yIO_wur _yIO_nn()
int _yIO_scan_string_literal_in(yio_scanctx_t *t, const char *str, const char *end);
