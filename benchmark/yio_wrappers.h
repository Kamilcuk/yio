#ifndef YIO_WRAPPERS_H
#define YIO_WRAPPERS_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

// Helper to get strerror
const char *wrap_yio_strerror(int err);

// Integers
int wrap_yio_int(char *buf, size_t sz, int val);
int wrap_libc_int(char *buf, size_t sz, int val);

// uint8_t / int8_t
int wrap_yio_u8(char *buf, size_t sz, uint8_t val);
int wrap_libc_u8(char *buf, size_t sz, uint8_t val);
int wrap_yio_i8(char *buf, size_t sz, int8_t val);
int wrap_libc_i8(char *buf, size_t sz, int8_t val);

// char
int wrap_yio_char(char *buf, size_t sz, char val);
int wrap_libc_char(char *buf, size_t sz, char val);

// uint16_t / int16_t
int wrap_yio_u16(char *buf, size_t sz, uint16_t val);
int wrap_libc_u16(char *buf, size_t sz, uint16_t val);
int wrap_yio_i16(char *buf, size_t sz, int16_t val);
int wrap_libc_i16(char *buf, size_t sz, int16_t val);

// long / unsigned long long
int wrap_yio_long(char *buf, size_t sz, long val);
int wrap_libc_long(char *buf, size_t sz, long val);
int wrap_yio_ull(char *buf, size_t sz, unsigned long long val);
int wrap_libc_ull(char *buf, size_t sz, unsigned long long val);

// int128 / uint128
#ifdef __SIZEOF_INT128__
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;
int wrap_yio_i128(char *buf, size_t sz, int128_t val);
int wrap_yio_u128(char *buf, size_t sz, uint128_t val);
#endif

// Floats
int wrap_yio_float(char *buf, size_t sz, float val);
int wrap_libc_float(char *buf, size_t sz, float val);
int wrap_yio_double(char *buf, size_t sz, double val);
int wrap_libc_double(char *buf, size_t sz, double val);
int wrap_yio_ldouble(char *buf, size_t sz, long double val);
int wrap_libc_ldouble(char *buf, size_t sz, long double val);

// Strings
int wrap_yio_str(char *buf, size_t sz, const char *s);
int wrap_libc_str(char *buf, size_t sz, const char *s);

// wchar_t / wchar_t*
int wrap_yio_wchar(char *buf, size_t sz, wchar_t val);
int wrap_libc_wchar(char *buf, size_t sz, wchar_t val);
int wrap_yio_wstr(char *buf, size_t sz, const wchar_t *s);
int wrap_libc_wstr(char *buf, size_t sz, const wchar_t *s);

#ifdef __cplusplus
}
#endif

#endif // YIO_WRAPPERS_H
