/*
 * yio.c
 *
 *  Created on: 29 wrz 2019
 *      Author: Kamil
 */
#define _POSIX_C_SOURCE  200809L
#include "yio.h"
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>


int yio_flags_from_fmt(struct yioopt_s *f, const char *str, yioctx_t *yioctx) {
	// regex:
	// [%]?[+- #0]*([0-9]*|\*)?(\.([0-9]*|\*)?)?
	//     actually precision_plus_1 is needed after comma, but anyway
	// ([l]?c|[l]?s|[hl]{0,2}[dioxXu]|[lL]?[fFeEaAgG]|n|p)

	// if first char is % ignore it
	if (*str == '%') {
		++str;
	}

	// conversion flag specifier
	for (; *str != '\0'; ++str) {
		bool dobreak = false;
		switch (*str) {
		case '+': f->flags |= YIO_SHOWPOS; break;
		case '-': f->flags |= YIO_LEFT; break;
		case ' ': f->fill = ' '; break;
		case '#': f->flags |= YIO_SHOWBASE; break;
		case '0': f->fill = '0'; break;
		default: dobreak = true; break;
		}
		if (dobreak) {
			break;
		}
	}

	// minimum field width
	if (*str == '*') {
		f->width = yioctx_va_arg(yioctx, int);
		str++;
	} else {
		int number = 0;
		for (; isdigit((unsigned char)*str); str++) {
			if (INT_MAX / 10 < number) {
				break;
			}
			number *= 10;
			const int to_add = *str - '0';
			if (INT_MAX - to_add < number) {
				break;
			}
			number += to_add;
		}
		f->width = number;
	}

	// precision_plus_1
	if (*str == '.') {
		str++;

		if (*str == '*') {
			f->precision = yioctx_va_arg(yioctx, int);
			str++;
		} else {
			int number = 0;
			for (; isdigit((unsigned char)*str); str++) {
				if (INT_MAX / 10 < number) {
					break;
				}
				number *= 10;
				const int to_add = *str - '0';
				if (INT_MAX - to_add < number) {
					break;
				}
				number += to_add;
			}
			f->precision = number;
		}
	}

	// ignore length modifier if any
	while (strchr("hlLm", *str) != NULL) {
		str++;
	}

	// conversion format specifier
	char conv_spec = *str;
	if (isupper((unsigned char)conv_spec)) {
		f->flags |= YIO_UPPERCASE;
		conv_spec = tolower(conv_spec);
	}
	switch (conv_spec) {
	case 'x': f->flags |= YIO_HEX; break;
	case 'o': f->flags |= YIO_OCTAL; break;
	case 'e': f->flags |= YIO_SCIENTIFIC; break;
	case 'a': f->flags |= YIO_FIXED; break;
	case 'g': f->flags |= YIO_FIXED | YIO_SCIENTIFIC; break;
	}

	return 0;
}

int yioflags_print(const struct yioopt_s *f) {
	return printf("flags %p: %08x %u %u %02x\n",
			(void*)f, f->flags, f->width, f->precision, f->fill);
}

int yio_flags_get_adjustment(const struct yioopt_s *f) {
	if (f->flags & YIO_INTERNAL) {
		return YIO_INTERNAL;
	}
	if (f->flags & YIO_LEFT) {
		return YIO_LEFT;
	}
	if (f->flags & YIO_RIGHT) {
		return YIO_RIGHT;
	}
	return YIO_LEFT;
}

int _yyy_yio_fmt(yioctx_t *t) {
	char * const fmt = yioctx_va_arg(t, char *);
	const int ret = yio_flags_from_fmt(yioctx_opt_nonconst(t), fmt, t);
	if (ret < 0) return ret;
	return yioctx_next(t);
}

/* -------------------------------------------------------------------------------------- */

__attribute__((__nonnull__))
static inline
void _yyy_yioctx_init(yioctx_t *t, FILE *file, const struct _yyy_yio_data_s *data, va_list *va) {
	assert(t != NULL);
	assert(data != NULL);
	assert(va != NULL);
	*t = (yioctx_t){
		.data = data,
		.file = file,
		.ifunc = &data->funcs[0],
		.va = va,
		.argsizespnt = &data->argsizes[0],
	};
	yioopt_init(&t->opt);
}

__attribute__((__nonnull__))
static inline
void _yyy_yioctx_add_written(yioctx_t *t, int count) {
	assert(t != NULL);
	assert(count >= 0);
	assert(INT_MAX - count > t->writtencnt);
	t->writtencnt += count;
}

static inline
void _yyy_yioctx_add_written_items(yioctx_t *t) {
	if (t->itemscntcb != NULL) {
		(*t->itemscntcb)++;
	}
}

__attribute__((__warn_unused_result__, __nonnull__))
static inline
int _yyy_yioctx_written(const yioctx_t *t) {
	return t->writtencnt;
}

/* -------------------------------------------------------------------------------------- */

int yvprint(_yio_data_t *data, va_list *va) {
	return yvprintf("", data, va);
}

int yvfprint(FILE *file, _yio_data_t *data, va_list *va) {
	return yvfprintf(file, "", data, va);
}

int yvsnprint(char *dest, size_t size, _yio_data_t *data, va_list *va) {
	return yvsnprintf(dest, size, "", data, va);
}

int _yyy_yprintf(const char *fmt, const struct _yyy_yio_data_s *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvprintf(fmt, data, &va);
	va_end(va);
	return ret;
}

int _yyy_yfprintf(FILE *file, const char *fmt, const struct _yyy_yio_data_s *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvfprintf(file, fmt, data, &va);
	va_end(va);
	return ret;
}

int _yyy_ysnprintf(char *dest, size_t size, const char *fmt, _yio_data_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvsnprintf(dest, size, fmt, data, &va);
	va_end(va);
	return ret;
}

/* --------------------------------------------------------------------------- */

int yvprintf(const char *fmt, _yio_data_t *data, va_list *va) {
	return yvfprintf(stdout, fmt, data, va);
}

int yvfprintf(FILE *file, const char *fmt, _yio_data_t *data, va_list *va) {
	assert(va != NULL);

	yioctx_t ctx = {0};
	if (data != NULL) {
		_yyy_yioctx_init(&ctx, file, data, va);
	}

	_yyy_func_t ifunc = data == NULL ? NULL : _yyy_yioctx_get_next_func(&ctx);
	while (ifunc != NULL || *fmt != '\0') {

		if (*fmt != '\0') {
			const char *pnt = fmt;

			// find "{}", but ignore "{{}"
			while (1) {
				pnt = strstr(pnt, "{}");
				if (pnt == NULL) {
					break;
				}
				if (pnt == fmt) {
					break;
				}
				if (*(pnt - 1) == '{') {
					pnt += 2;
					continue;
				}
				break;
			}

			const size_t len = pnt == NULL ? strlen(fmt) : (size_t)(pnt - fmt);
			const size_t ret = fwrite(fmt, len, 1, file);
			if (ret != 1) {
				return -1;
			}

			_yyy_yioctx_add_written(&ctx, len);

			fmt += len + (pnt != NULL ? 2 : 0);
		}

		if (data != NULL && ifunc != NULL) {

			const int ret = ifunc(&ctx);
			if (ret < 0) {
				return ret;
			}

			_yyy_yioctx_add_written(&ctx, ret);
			_yyy_yioctx_add_written_items(&ctx);
			if (!(yioctx_opt(&ctx)->flags & YIO_PERSISTENT)) {
				yioopt_init(&ctx.opt);
			}
			if (yioctx_opt(&ctx)->flags & YIO_UNITBUF) {
				fflush(yioctx_file(&ctx));
			}

			ifunc = _yyy_yioctx_get_next_func(&ctx);
		}
	}

	assert(*ctx.argsizespnt == 0);
	// ctx.ifunc is set one past element of the array and last element is null
	assert(*(ctx.ifunc - 1) == NULL);

	return _yyy_yioctx_written(&ctx);
}

int yvsnprintf(char *dest, size_t size, const char *fmt, _yio_data_t *data, va_list *va) {
	FILE * const f = fmemopen(dest, size, "w");
	if (f == NULL) {
		return -ENOMEM;
	}
	const int ret = yvfprintf(f, fmt, data, va);
	fclose(f);
	return ret;
}

/* ------------------------------------------------------------------------------------------- */

int _yio_unittest(void) {
	yprintf("one is equal to: {{} {} \n", 1);

	yprint("B", 1, "A", 1.0f, "\n");

	yprint("endl?", yio_endl());

	yprint("endl?", yio_endl());

	yprintf("This is not {} a command\n");
	yprintf("one is = {}, float formatted is {:+# 10.6}\n", 1, 1.0f);
	struct custom { int a; } custom;
	int custom_yioctx(yioctx_t *t);
#define custom_yio(arg) _yio_cb(custom_yioctx, _Generic((arg), struct custom*:(arg)))
	struct custom inst;
	yprintf("custom format = {}", custom_yio(inst));

	format("{:*^{}.{}%}", 1., 15, 2); // With fill, dynamic width and dynamic precision.
	// Result: "****100.00%****"

	yprintf("one is equal to: {}\n", 1);
#if 1
	yprint(1, yiofmt("+#*.*", 20, 10), 1.0f, " AA \n");
	yprint(1, yiofmt("+#*.*", 20, 1), 1.0f, " AA \n");

	yprintf("number = {:+#*.*x}", 20, 1, 1.0f, "\n");

#define test_in(args, value) do { \
	printf("TEST: " #value " = \n"); \
	value; \
	printf("\n"); \
} while (0)
#define test(a) test_in(#a, (yprint a));

	test(((char)'a'));
	test(((signed char)'a'));
	test(((unsigned char)'a'));
	test(((short)48));
	test(((unsigned short)48));
	test(((int)48));
	test(((unsigned int)48));
	test(((float)1.2345678));
	test(((double)1.2345678));
	test(((long double)1.2345678));

	{
		test((yiofmt("#0*X", 10), 123));
		const int spec = 10;
		test((yiofmt("#0*X", spec), 123));
		const char *fmt = "#0*X";
		test((yiofmt(fmt, spec), 123));
		const char arr[] = "#0*X";
		test((yiofmt(arr, spec), 123));
		char narr[] = "#0*X";
		test((yiofmt(narr, spec), 123));
		char nfmt[] = "#0*X";
		test((yiofmt(nfmt, spec), 123));
		test((yiofmt(nfmt - 1 + 1, ((void)0, spec - 1 + 1)), 123));
	}
	{
		test(("endl?", yio_endl()));
	}

	double a = 3.1415926534;
	double b = 2006.0;
	double c = 1.0e-10;

	test((yio_persistent(), yio_precision(5), a, "\n", b, "\n", c, "\n"));
	test((yio_persistent(), yio_precision(5), yio_fixed(), a, "\n", b, "\n", c, "\n"));
	test((yio_persistent(), yio_precision(5), yio_scientific(), a, "\n", b, "\n", c, "\n"));
	test((yio_persistent(), yio_precision(5), yio_hexfloat(), a, "\n", b, "\n", c, "\n"));
#endif
	return 0;
}





