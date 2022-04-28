/**
 * @file
 * @date 2020-06-17
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
// Type your code here.
#define _GNU_SOURCE
#include <yio_test.h>
#include <yπio.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <limits.h>

/* ------------------------------------------------------------------------- */

/* intprops.h -- properties of integer types
   Copyright (C) 2001-2019 Free Software Foundation, Inc.
   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */
/* True if the real type T is signed.  */
#define TYPE_SIGNED(t) (! ((t) 0 < (t) -1))
/* Minimum and maximum values for integer types and expressions.  */
/* The width in bits of the integer type or expression T.
   Do not evaluate T.
   Padding bits are not supported; this is checked at compile-time below.  */
#define TYPE_WIDTH(t) (sizeof (t) * CHAR_BIT)
/* The maximum and minimum values for the integer type T.  */
#define TYPE_MINIMUM(t) ((t) ~ TYPE_MAXIMUM (t))
#define TYPE_MAXIMUM(t)                                                 \
  ((t) (! TYPE_SIGNED (t)                                               \
        ? (t) -1                                                        \
        : ((((t) 1 << (TYPE_WIDTH (t) - 2)) - 1) * 2 + 1)))

/* ------------------------------------------------------------------------- */

#define TEST_in(type, value) \
    do { \
        type _var = value; \
        TCHAR *_ptr = yπformat("("#type")"#value" -> ", _var, "\n"); \
        yπprint(_ptr); \
        _yIO_TEST(TSTRCMP(_ptr, #value) == 0, "%"TPRI" failed", _ptr); \
        free(_ptr); \
    } while(0)
#undef TEST_in
#define TEST_in(type, value) \
    do { \
        type _var = value; \
        yπprint("(", #type ")(", #value, ") = ", _var, "\n"); \
    } while(0)


#define TEST(type, value)  TEST_in(type, value)

#define TESTINT(type)  do { \
		TEST(type, TYPE_MAXIMUM(type)); \
		TEST(type, TYPE_MINIMUM(type)); \
		} while (0)

static void test_fundamental_types(void) {
    TESTINT(short);
    TESTINT(short int);
    TESTINT(signed short);
    TESTINT(signed short int);
    TESTINT(unsigned short);
    TESTINT(unsigned short int);
    TESTINT(int);
    TESTINT(signed);
    TESTINT(signed int);
    TESTINT(unsigned);
    TESTINT(unsigned int);
    TESTINT(long);
    TESTINT(long int);
    TESTINT(signed long);
    TESTINT(signed long int);
    TESTINT(unsigned long);
    TESTINT(unsigned long int);
    TESTINT(long long);
    TESTINT(long long int);
    TESTINT(signed long long);
    TESTINT(signed long long int);
    TESTINT(unsigned long long);
    TESTINT(unsigned long long int);
    TEST(_Bool, 0);
    TEST(_Bool, 1);
    TEST(_Bool, 2);
}

static void test_std_types(void) {
	// stdint.h
	TESTINT(int8_t);
	TESTINT(uint8_t);
	TESTINT(int16_t);
	TESTINT(uint16_t);
	TESTINT(int32_t);
	TESTINT(uint32_t);
	TESTINT(int64_t);
	TESTINT(uint64_t);
	TESTINT(int_fast8_t);
	TESTINT(uint_fast8_t);
	TESTINT(int_fast16_t);
	TESTINT(uint_fast16_t);
	TESTINT(int_fast32_t);
	TESTINT(uint_fast32_t);
	TESTINT(int_fast64_t);
	TESTINT(uint_fast64_t);
	TESTINT(int_least8_t);
	TESTINT(uint_least8_t);
	TESTINT(int_least16_t);
	TESTINT(uint_least16_t);
	TESTINT(int_least32_t);
	TESTINT(uint_least32_t);
	TESTINT(int_least64_t);
	TESTINT(uint_least64_t);
	TESTINT(intmax_t);
	TESTINT(uintmax_t);
	TESTINT(intptr_t);
	TESTINT(uintptr_t);
	// time.h
	TESTINT(clock_t);
	TESTINT(time_t);
	// signal.h
	TESTINT(sig_atomic_t);
	// stdio.h
//	TESTINT(fpos_t);
	// stddef.h
	TESTINT(size_t);
	TESTINT(ptrdiff_t);
}

#if _yIO_HAS___int128
#define INT128_MAX (__int128)(((unsigned __int128) 1 << ((sizeof(__int128) * __CHAR_BIT__) - 1)) - 1)
#define INT128_MIN (-INT128_MAX - 1)
#define UINT128_MAX ((2 * (unsigned __int128) INT128_MAX) + 1)
static void test_int128(void) {
    TEST(__int128, INT128_MAX);
    TEST(__int128, INT128_MIN);
    TEST(unsigned __int128, UINT128_MAX);
}
#else
static void test_int128(void) {}
#endif

#if _yIO_HAS_UNISTD_H
#include <sys/types.h>
static void test_posix_types(void) {
    TESTINT(blkcnt_t);
    TESTINT(blksize_t);
    TESTINT(clock_t);
    TESTINT(clockid_t);
    TESTINT(dev_t);
    TESTINT(fsblkcnt_t);
    TESTINT(fsfilcnt_t);
    TESTINT(gid_t);
    TESTINT(id_t);
    TESTINT(ino_t);
    TESTINT(mode_t);
    TESTINT(nlink_t);
    TESTINT(off_t);
    TESTINT(pid_t);
    TESTINT(size_t);
    TESTINT(ssize_t);
    TESTINT(suseconds_t);
    TESTINT(time_t);
    // Implementators define it to void*
    // TESTINT(timer_t);
    TESTINT(uid_t);
    // TESTINT(useconds_t);
}
#else
static void test_posix_types(void) {}
#endif

int main() {
	test_fundamental_types();
	test_std_types();
	test_int128();
	test_posix_types();
}

