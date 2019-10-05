/*
 * yio_print.c
 *
 *  Created on: 5 paŸ 2019
 *      Author: Kamil
 */
#include "yio.h"

#define YIO_FMT_BUFSIZE  5+3+24+24+1

static inline
char *yio_flags_to_fmt(char buf[YIO_FMT_BUFSIZE], const struct yioopt_s *f,
		const char *length_mod, char conv_spec) {
	const size_t bufsize = YIO_FMT_BUFSIZE;
	char *pnt = buf;
	*pnt++ = '%';

	if (f->flags & YIO_SHOWPOS) *pnt++ = '+';
	if (f->flags & YIO_LEFT) *pnt++ = '-';
	if (f->flags & YIO_SHOWBASE) *pnt++ = '#';
	if (f->fill == '0' || f->fill == ' ') *pnt++ = f->fill;

	if (f->width >= 0) {
		pnt += snprintf(pnt, bufsize - (pnt - buf), "%d", f->width);
	}

	if (f->precision >= 0) {
		*pnt++ = '.';
		pnt += snprintf(pnt, bufsize - (pnt - buf), "%d", f->precision);
	}

	if (length_mod != NULL) {
		if (*length_mod != '\0') {
			*pnt++ = *length_mod++;
			if (*length_mod != '\0') {
				*pnt++ = *length_mod++;
			}
		}
		assert(*length_mod == '\0');
	}

	if (conv_spec != 0) {
		*pnt++ = conv_spec;
	}

	*pnt++ = '\0';
	assert((uintptr_t)(pnt - buf) < bufsize);

	return buf;
}

static inline
int _yyy_print_smaller_then_int(yioctx_t *t, int arg) {
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), NULL,
				yioctx_opt(t)->flags & YIO_HEX ? yioctx_opt(t)->flags & YIO_UPPERCASE ? 'X' : 'x' : 'd');
	return fprintf(yioctx_file(t), fmt, arg);
}

static inline
int _yyy_print_smaller_then_uint(yioctx_t *t, unsigned arg) {
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), NULL, 'u');
	return fprintf(yioctx_file(t), fmt, arg);
}

static inline
int _yyy_print_generic_char(yioctx_t *t, int arg) {
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), NULL, 'c');
	return fprintf(yioctx_file(t), fmt, arg);

}

int _yyy_print_char(yioctx_t *t) {
	const int arg = yioctx_va_arg2(t, char, int);
	return _yyy_print_generic_char(t, arg);
}

int _yyy_print_schar(yioctx_t *t) {
	const int arg = yioctx_va_arg2(t, signed char, int);
	return _yyy_print_generic_char(t, arg);
}

int _yyy_print_uchar(yioctx_t *t) {
	const int arg = yioctx_va_arg2(t, unsigned char, int);
	return _yyy_print_generic_char(t, arg);
}

int _yyy_print_short(yioctx_t *t) {
	const int arg = yioctx_va_arg2(t, short, int);
	return _yyy_print_smaller_then_int(t, arg);
}

int _yyy_print_ushort(yioctx_t *t) {
	const unsigned arg = yioctx_va_arg2(t, unsigned short, int);
	return _yyy_print_smaller_then_uint(t, arg);
}

int _yyy_print_int(yioctx_t *t) {
	const int arg = yioctx_va_arg(t, int);
	return _yyy_print_smaller_then_int(t, arg);
}

int _yyy_print_uint(yioctx_t *t) {
	const unsigned arg = yioctx_va_arg(t, unsigned);
	return _yyy_print_smaller_then_uint(t, arg);
}

int _yyy_print_long(yioctx_t *t) {
	const long arg = yioctx_va_arg(t, long);
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), "l", 'd');
	return fprintf(yioctx_file(t), fmt, arg);
}

int _yyy_print_ulong(yioctx_t *t) {
	const unsigned long arg = yioctx_va_arg(t, unsigned long);
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), "l", 'u');
	return fprintf(yioctx_file(t), fmt, arg);
}

int _yyy_print_llong(yioctx_t *t) {
	const long long arg = yioctx_va_arg(t, long long);
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), "ll", 'd');
	return fprintf(yioctx_file(t), fmt, arg);
}

int _yyy_print_ullong(yioctx_t *t) {
	const unsigned long long arg = yioctx_va_arg(t, unsigned long long);
	if (yioctx_file(t) == NULL) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), "ll", 'u');
	return fprintf(yioctx_file(t), fmt, arg);
}

static inline
int _yyy_asis_int(int arg) {
	return arg;
}

static inline
char _yyy_print_get_float_conv_spec(yioctx_t *t) {
	const struct yioopt_s * const f = yioctx_opt(t);
	const unsigned flags = f->flags;
	return (flags & YIO_UPPERCASE ? toupper : _yyy_asis_int)((unsigned char)(
			(flags & YIO_FIXED) && (flags & YIO_SCIENTIFIC) ? 'a' :
			(flags & YIO_SCIENTIFIC) ? 'e' :
					(flags & YIO_FIXED) ? 'f' :
							'g'));
}

static inline
int _yyy_print_double_generic(yioctx_t *t, double arg) {
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), "",
			_yyy_print_get_float_conv_spec(t));
	return fprintf(yioctx_file(t), fmt, arg);
}

int _yyy_print_float(yioctx_t *t) {
	const double arg = yioctx_va_arg2(t, float, double);
	return _yyy_print_double_generic(t, arg);
}

int _yyy_print_double(yioctx_t *t) {
	const double arg = yioctx_va_arg2(t, double, double);
	return _yyy_print_double_generic(t, arg);
}

int _yyy_print_ldouble(yioctx_t *t) {
	const long double arg = yioctx_va_arg2(t, long double, long double);
	if (yioctx_no_file(t)) return 0;
	const struct yioopt_s *f = yioctx_opt(t);
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, f, "L",
			_yyy_print_get_float_conv_spec(t));
	return fprintf(yioctx_file(t), fmt, arg);
}

static inline
int _yyy_print_constornot_charpnt(yioctx_t *t, const char *arg) {
	if (yioctx_no_file(t)) return 0;
	const char * const fmt = yio_flags_to_fmt((char[YIO_FMT_BUFSIZE]){0}, yioctx_opt(t), NULL, 's');
	return fprintf(yioctx_file(t), fmt, arg);
}

int _yyy_print_charpnt(yioctx_t *t) {
	const char * const arg = yioctx_va_arg(t, char *);
	return _yyy_print_constornot_charpnt(t, arg);
}

int _yyy_print_constcharpnt(yioctx_t *t) {
	const char * const arg = yioctx_va_arg(t, const char *);
	return _yyy_print_constornot_charpnt(t, arg);
}

int _yyy_print_endl(yioctx_t *t) {
	if (yioctx_no_file(t)) return 0;
	int ret = fprintf(yioctx_file(t), "\n");
	if (ret < 0) return ret;
	ret = fflush(yioctx_file(t));
	if (ret < 0) return ret;
	return 1;
}

int _yyy_print_flush(yioctx_t *t) {
	if (yioctx_no_file(t)) return 0;
	return fflush(yioctx_file(t));
}
