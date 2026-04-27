#include "yio_wrappers.h"
#include <stdio.h>
#include <yio.h>

const char *wrap_yio_strerror(int err) {
    return yio_strerror(err);
}

// Integers
int wrap_yio_int(char *buf, size_t sz, int val) {
    return yio_snprintf(buf, sz, "{}", val);
}

int wrap_libc_int(char *buf, size_t sz, int val) {
    return snprintf(buf, sz, "%d", val);
}

// uint8_t / int8_t
int wrap_yio_u8(char *buf, size_t sz, uint8_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_u8(char *buf, size_t sz, uint8_t val) {
    return snprintf(buf, sz, "%u", (unsigned int)val);
}
int wrap_yio_i8(char *buf, size_t sz, int8_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_i8(char *buf, size_t sz, int8_t val) {
    return snprintf(buf, sz, "%d", (int)val);
}

// char
int wrap_yio_char(char *buf, size_t sz, char val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_char(char *buf, size_t sz, char val) {
    return snprintf(buf, sz, "%c", val);
}

// uint16_t / int16_t
int wrap_yio_u16(char *buf, size_t sz, uint16_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_u16(char *buf, size_t sz, uint16_t val) {
    return snprintf(buf, sz, "%u", (unsigned int)val);
}
int wrap_yio_i16(char *buf, size_t sz, int16_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_i16(char *buf, size_t sz, int16_t val) {
    return snprintf(buf, sz, "%d", (int)val);
}

// long / unsigned long long
int wrap_yio_long(char *buf, size_t sz, long val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_long(char *buf, size_t sz, long val) {
    return snprintf(buf, sz, "%ld", val);
}
int wrap_yio_ull(char *buf, size_t sz, unsigned long long val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_ull(char *buf, size_t sz, unsigned long long val) {
    return snprintf(buf, sz, "%llu", val);
}

// int128 / uint128
#ifdef __SIZEOF_INT128__
int wrap_yio_i128(char *buf, size_t sz, int128_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_yio_u128(char *buf, size_t sz, uint128_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
#endif

// Floats
int wrap_yio_float(char *buf, size_t sz, float val) {
    return yio_snprintf(buf, sz, "{:f}", val);
}
int wrap_libc_float(char *buf, size_t sz, float val) {
    return snprintf(buf, sz, "%f", (double)val);
}
int wrap_yio_double(char *buf, size_t sz, double val) {
    return yio_snprintf(buf, sz, "{:f}", val);
}
int wrap_libc_double(char *buf, size_t sz, double val) {
    return snprintf(buf, sz, "%f", val);
}
int wrap_yio_ldouble(char *buf, size_t sz, long double val) {
    return yio_snprintf(buf, sz, "{:f}", val);
}
int wrap_libc_ldouble(char *buf, size_t sz, long double val) {
    return snprintf(buf, sz, "%Lf", val);
}

// Strings
int wrap_yio_str(char *buf, size_t sz, const char *s) {
    return yio_snprintf(buf, sz, "{}", s);
}
int wrap_libc_str(char *buf, size_t sz, const char *s) {
    return snprintf(buf, sz, "%s", s);
}

// wchar_t / wchar_t*
int wrap_yio_wchar(char *buf, size_t sz, wchar_t val) {
    return yio_snprintf(buf, sz, "{}", val);
}
int wrap_libc_wchar(char *buf, size_t sz, wchar_t val) {
    return snprintf(buf, sz, "%lc", (wint_t)val);
}
int wrap_yio_wstr(char *buf, size_t sz, const wchar_t *s) {
    return yio_snprintf(buf, sz, "{}", s);
}
int wrap_libc_wstr(char *buf, size_t sz, const wchar_t *s) {
    return snprintf(buf, sz, "%ls", s);
}
