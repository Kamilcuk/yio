/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef _yIO_YIO_YΩIO_FMT_PRIVATE_H_
#define _yIO_YIO_YΩIO_FMT_PRIVATE_H_
#include "../yio_common.h"

/**
 * @ingroup printfmt
 * The default values of printfmt
 */
extern const struct yπio_printfmt_s _yΩIO_printfmt_default;

/**
 * Parse pythong formatting string
 * @param c
 * @param pf
 * @param fmt Python formatting string like "{: < -0#123_.456}"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
_yIO_wur _yIO_nn()
int _yΩIO_pfmt_parse(struct _yΩIO_printctx_s *c, struct yπio_printfmt_s *pf,
		const TCHAR *fmt, const TCHAR **endptr);

/**
 * @ingroup printmod
 * @param t
 * @param str The number printed as a string without leading sign.
 * @param positive Is the number positive or negative?
 * @return 0 on success, error otherwise
 */
_yIO_wur _yIO_nn()
int _yΩIO_printformat_number(yπio_printctx_t *t, const TCHAR str[], bool positive);

/**
 * @ingroup printmod
 * @param t
 * @param str A string.
 * @return 0 on success, error otherwise
 */
_yIO_wur _yIO_nn()
int _yΩIO_printformat_string(yπio_printctx_t *t, const TCHAR str[]);


int _yΩIO_printctx_stdintparam(yπio_printctx_t *t,
		const TCHAR *ptr, const TCHAR **endptr, int *res);
bool _yΩIO_strnulchrbool(const TCHAR *s, TCHAR c);
#endif /* _yIO_YIO_YΩIO_FMT_PRIVATE_H_ */
