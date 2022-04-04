/**
 * @file
 * @date 2020-lip-29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

/**
 * Parse C formatting strnig
 * @param c
 * @param pf
 * @param fmt C format string like "%+- #123.456s"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
_yIO_wur _yIO_nn()
int _yΩIO_cfmt_parse(struct _yΩIO_printctx_s *c, struct yπio_printfmt_s *pf,
		const Ychar *fmt, const Ychar **endptr);

int _yΩIO_print_cfmt(yπio_printctx_t *t);

int _yΩIO_print_cfmt(yπio_printctx_t *t);


#define _yΩIO_ypcfmt_0(fmt)                _Generic((fmt),Ychar:(fmt)),
#define _yΩIO_ypcfmt_1(fmt, spec)          _Generic((fmt),Ychar:(fmt)), _Generic((spec),int:(spec))
#define _yΩIO_ypcfmt_2(fmt, spec, spec2)   _Generic((fmt),Ychar:(fmt)), _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yΩIO_ypcfmt_N(_0,_1,_2,N,...)  _yΩIO_ypcfmt_##N
/**
 * Format output by reading C format string.
 * @param ... C formatting string like "%s".
 * @param ... Optional up to two int values.
 */
#define yπpcfmt(...)  yiocb(_yΩIO_print_cfmt, _yΩIO_ypcfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))
