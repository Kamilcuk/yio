/**
 * @file yio_static_assert.h
 * @date 2020-05-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <assert.h>

#ifndef static_assert
#pragma GCC dianostic ignored "-Wunused-local-typedefs"
#define _YIO_STATIC_ASSERT(COND, MSG)  typedef char _static_assertion_##MSG[(!!(COND))*2-1]
#define _YIO_COMPILE_TIME_ASSERT3(X,L)  _YIO_STATIC_ASSERT(X,static_assertion_at_line_##L)
#define _YIO_COMPILE_TIME_ASSERT2(X,L)  _YIO_COMPILE_TIME_ASSERT3(X,L)
#define _YIO_COMPILE_TIME_ASSERT(X)     _YIO_COMPILE_TIME_ASSERT2(X,__LINE__)
#define static_assert(expr, msg)  _YIO_COMPILE_TIME_ASSERT(expr)
#endif
