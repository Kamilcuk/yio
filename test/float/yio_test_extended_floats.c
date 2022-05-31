/*
 * yio_test_extendedfloat_1.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#include <yio.h>
#include <float.h>
#define _(x)  yprint( #x "123.456=", (x)123.456, "\n");
int main() {
#if YYIO_USE__FLOATN
#ifdef FLT16_DECIMAL_DIG
	_(_Float16);
#endif
#ifdef FLT32_DECIMAL_DIG
	_(_Float32);
#endif
#ifdef FLT64_DECIMAL_DIG
	_(_Float64);
#endif
#ifdef FLT128_DECIMAL_DIG
	_(_Float128);
#endif
#ifdef FLT32X_DECIMAL_DIG
	_(_Float32x);
#endif
#ifdef FLT64X_DECIMAL_DIG
	_(_Float64x);
#endif
#ifdef FLT128X_DECIMAL_DIG
	_(_Float128x);
#endif
#endif // #if YYIO_USE__FLOATN
#if YYIO_USE__DECIMALN
#ifdef DEC32_DECIMAL_DIG
	_(_Decimal32);
#endif
#ifdef DEC64_DECIMAL_DIG
	_(_Decimal64);
#endif
#ifdef DEC128_DECIMAL_DIG
	_(_Decimal128);
#endif
#endif // #if YYIO_USE__DECIMALN
}
