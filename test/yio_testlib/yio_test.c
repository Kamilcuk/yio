/**
 * @file yio_test.c
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_test_private.h"

#if __arm__ && __GNUC__
extern void initialise_monitor_handles(void);
__attribute__((__constructor__))
static void init() {
    initialise_monitor_handles();
}
#endif

#ifdef YIO_HAS_LIBDFP
extern int register_printf_dfp(void);
__attribute__((constructor))
static void init_libdfp(void) {
	register_printf_dfp();
}
#endif

static int yyio_hexdigit(int c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return -1;
}

bool YIO_test_float_equal(const char *ref, const char *res, char spec) {
	bool match = strcmp(ref, res) == 0;
	if (!match && (spec == 'a' || spec == 'A')) {
		match = yio_test_hexf_eq(ref, res);
	}
	return match;
}

bool YIO_test_string_float_equal(struct YIO_string *ref, struct YIO_string *res, char spec) {
	bool match = YIO_string_equal(ref, res);
	if (!match && (spec == 'a' || spec == 'A')) {
		match = yio_test_hexf_eq(YIO_string_c_str(ref), YIO_string_c_str(res));
	}
	return match;
}

bool yio_test_hexf_eq(const char *s1, const char *s2) {
	struct {
		int sign;
		uint64_t m;
		long e;
		bool ok;
	} h[2];

	const char *s[2] = {s1, s2};

	for (int i = 0; i < 2; i++) {
		const char *p = s[i];
		h[i].ok = false;
		h[i].sign = 1;
		h[i].m = 0;
		h[i].e = 0;

		if (!p) return false;

		while (isspace((unsigned char)*p)) p++;
		if (*p == '-') { h[i].sign = -1; p++; }
		else if (*p == '+') p++;

		if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X')) {
			continue;
		}
		p += 2;

		int f = 0;
		bool dot = false;
		while (isxdigit((unsigned char)*p) || *p == '.') {
			if (*p == '.') {
				if (dot) { h[i].ok = false; break; }
				dot = true;
			} else {
				h[i].m = (h[i].m << 4) | (uint64_t)yyio_hexdigit(*p);
				if (dot) f++;
			}
			p++;
			h[i].ok = true;
		}

		if (h[i].ok && (*p == 'p' || *p == 'P')) {
			p++;
			char *end;
			h[i].e = strtol(p, &end, 10);
			h[i].e -= 4L * f;
		} else {
			h[i].e = -4L * f;
		}
	}

	if (!h[0].ok || !h[1].ok) {
		return strcmp(s1, s2) == 0;
	}

	if (h[0].sign != h[1].sign) {
		if (h[0].m == 0 && h[1].m == 0) return true;
		return false;
	}
	if (h[0].m == 0 && h[1].m == 0) return true;

	if (h[0].e > h[1].e) {
		long diff = h[0].e - h[1].e;
		if (diff >= 64) return false;
		h[0].m <<= diff;
	} else if (h[1].e > h[0].e) {
		long diff = h[1].e - h[0].e;
		if (diff >= 64) return false;
		h[1].m <<= diff;
	}

	return h[0].m == h[1].m;
}
