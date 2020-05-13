/**
 * @file yio_macros_priv.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio_config.h"

#if _yIO_HAVE_FLOAT_H
#if _yIO_USE__FLOATN && !defined(__STDC_WANT_IEC_60559_TYPES_EXT__)
#define __STDC_WANT_IEC_60559_TYPES_EXT__  1
#endif
#if _yIO_USE__DECIMALN && !defined(__STDC_WANT_DEC_FP__)
#define __STDC_WANT_DEC_FP__  1
#endif
#if _yIO_USE__FLOATN || _yIO_USE__DECIMALN
#include <float.h>
#endif
#endif

#include "yio_macros_gen.h"
#include <stddef.h>

/* ----------------------------------------------------------------------------------------------- */

// As of now (2020-03) eclipse doesn't understand _Generic, so disable it.
#ifdef __CDT_PARSER__
#define _Generic(a, ...)   _G(a)
#endif

/**
 * @def _yIO_GENERIC
 * _Generic replacement so that we can overload it in C++
 */
#ifdef __cplusplys
#error TODO: private _Generic alternative in C++ using typeid.type comparision \
		or just lambda dynamic dispatch
#define _yIO_GENERIC(TYPE, ...)
#else
#define _yIO_GENERIC(TYPE, ...)  _Generic(TYPE, _yIO_GENERIC_GEN(__VA_ARGS__))
#endif

/* Private Macros -------------------------------------------------------------------------------- */

/**
 * Re-evaulate the arguments
 */
#define _yIO_ESC(...)  __VA_ARGS__

/**
 * Ignore first argument
 */
#define _yIO_IGNORE1_IN(_1,...)  __VA_ARGS__
#define _yIO_IGNORE1(...)        _yIO_IGNORE1_IN(__VA_ARGS__,0)

/**
 * @brief Commas
 * @{
 */
#define _yIO_COMMA    ,
#define _yIO_58COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define _yIO_59COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define _yIO_60COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define _yIO_61COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
/**
 * @}
 */


/**
 * Is whatever you give a character string literal without a cast
 * https://en.cppreference.com/w/c/language/string_literal
 * @{
 */
#if 0
#define _yIO_IS_CHARACTER_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>2&&#x[0]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_UTF_8_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>4&&#x[0]=='u'&&#x[1]=='8'&&#x[2]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_16_BIT_WIDE_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>3&&#x[0]=='u'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_32_BIT_WIDE_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>3&&#x[0]=='U'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_WIDE_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>3&&#x[0]=='L'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_STRING_LITERAL_OLD(x)(\
		sizeof(#x)>2&&#x[sizeof(#x)-2]=='"'&&(\
				#x[0]=='"'||(sizeof(#x)>3&&(#x[0]=='u'||#x[0]=='U'||#x[0]=='L')&&#x[1]=='"')\
				||\
				(sizeof(#x)>4&&#x[0]=='u'&&#x[1]=='8'&&#x[2]=='"')\
		)\
)
#endif

static inline
int _yIO__is_wide_string_literal(const char * const x, const size_t sizeof_x) {
	return sizeof_x > 3 && x[0] == 'L' && x[1] == '"' && x[sizeof_x - 2] == '"';
}
/**
 * Detect if @b x is a wide string literal.
 * @param x a wide string literal like L"abc" or anything else
 * @return nonzero if @b x is a wide string literal, anything else otherwise
 */
#define _yIO_IS_WIDE_STRING_LITERAL(x)  _yIO__is_wide_string_literal(#x, sizeof(#x))

static inline
int _yIO__is_string_literal(const char * const x, const size_t sizeof_x) {
	return sizeof_x > 2 && x[sizeof_x - 2] == '"' && (
					x[0] == '"' || (
							sizeof_x > 3 && (
									x[0] == 'u' ||
									x[0] == 'U' ||
									x[0] == 'L'
							) &&
							x[1] == '"'
					) || (
							sizeof_x > 4 &&
							x[0] == 'u' &&
							x[1] == '8' &&
							x[2] == '"'
					)
			);
}
/**
 * Detect if @b x is a string literal.
 * @param x Can be a string literal like u8"abc" or L"def" or anything else.
 * @return nonzero if @b x is any kind of string literal, anything else otherwise
 */
#define _yIO_IS_STRING_LITERAL(x)  _yIO__is_string_literal(#x, sizeof(#x))

/**
 * @}
 */

/**
 * _yIO_IFBA62A
 * If braces and 62 or more arguments then 'then' else 'else'
 */
#define _yIO_IFBA62A_0(then, else)   else
#define _yIO_IFBA62A_62(then, else)  then
#define _yIO_IFBA62A_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,N,...) _yIO_IFBA62A_##N
#define _yIO_IFBA62A_IN(expr) _yIO_IFBA62A_N(expr,62,62,62,62,62,62,62,62,62,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define _yIO_IFBA62A(expr, then, else)  _yIO_IFBA62A_IN(_yIO_ESC _1)(then, else)

/**
 * _yIO_JFB62TE
 * Join for each argument iF braces with 62 arguments or more apply 'Then' else apply 'Else' also pass separators along
 *
 *     This function is basically the hearth of everything that happens here.
 *     The 'before' and 'after' are separators that are used to join the list.
 *     First two arguments should be inside braces.
 *     The 'then' function may choose to not expand before nor after,
 *     if the function sees it fit. The should be expanded with _yIO_ESC.
 *     Special care needs to be taken to conform to ISO C99, that at least one argument needs to go to ellipsis.
 *
 *     Running:
 *        #define FUNC_ON_62_ARGS(before, after, arg1, arg2, ...)  \
 *                _yIO_ESC before RESULT(arg1, arg2, __VA_ARGS__) _yIO_ESC after
 *        #define FUNC_ON_LESS_THEN_62_ARGS(arg)  RESULT(arg)
 *        _yIO_JFB62TE((BEFORE), (AFTER),
 *                     FUNC_ON_62_ARGS, FUNC_ON_LESS_THEN_62_ARGS,
 *                     ARG1,
 *                     ARG2,
 *                     ((ARG3), _yIO_60COMMAS),
 *                     ((ARG4, ARG5, ARG6),
 *                     _yIO_60COMMAS))
 *
 * Runs:
 *   - foreach argument
 *     - if that argument doesn't has 62 arguments
 *        - do then(before, after, UNESCAPE(GET_FIRST_ARGUMENT(UNESCAPE(argment))) )
 *     - else
 *        - do _yIO_ESC before else(argument) _yIO_ESC after
 *
 */
#define _yIO_JFB62TE_DP_IN(b,a,t,_1,...) t(b,a,_1)
#define _yIO_JFB62TE_DP(b,a,t,args) _yIO_JFB62TE_DP_IN(b,a,t,_yIO_ESC args)
#define _yIO_JFB62TE_1(b,a,t,e,_1)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)
#define _yIO_JFB62TE_2(b,a,t,e,_1,_2)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)
#define _yIO_JFB62TE_3(b,a,t,e,_1,_2,_3)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)
#define _yIO_JFB62TE_4(b,a,t,e,_1,_2,_3,_4)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _4)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _4),_yIO_ESC b e(_4) _yIO_ESC a)
#define _yIO_JFB62TE_5(b,a,t,e,_1,_2,_3,_4,_5)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _4)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _4),_yIO_ESC b e(_4) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _5)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _5),_yIO_ESC b e(_5) _yIO_ESC a)
#define _yIO_JFB62TE_6(b,a,t,e,_1,_2,_3,_4,_5,_6)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _4)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _4),_yIO_ESC b e(_4) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _5)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _5),_yIO_ESC b e(_5) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _6)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _6),_yIO_ESC b e(_6) _yIO_ESC a)
#define _yIO_JFB62TE_7(b,a,t,e,_1,_2,_3,_4,_5,_6,_7)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _4)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _4),_yIO_ESC b e(_4) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _5)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _5),_yIO_ESC b e(_5) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _6)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _6),_yIO_ESC b e(_6) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _7)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _7),_yIO_ESC b e(_7) _yIO_ESC a)
#define _yIO_JFB62TE_8(b,a,t,e,_1,_2,_3,_4,_5,_6,_7,_8)_yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _4)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _4),_yIO_ESC b e(_4) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _5)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _5),_yIO_ESC b e(_5) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _6)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _6),_yIO_ESC b e(_6) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _7)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _7),_yIO_ESC b e(_7) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _8)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _8),_yIO_ESC b e(_8) _yIO_ESC a)
#define _yIO_JFB62TE_9(b,a,t,e,_1,_2,_3,_4,_5,_6,_7,_8,_9)  _yIO_IFBA62A_IN(_yIO_ESC _1)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _1),_yIO_ESC b e(_1) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _2)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _2),_yIO_ESC b e(_2) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _3)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _3),_yIO_ESC b e(_3) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _4)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _4),_yIO_ESC b e(_4) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _5)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _5),_yIO_ESC b e(_5) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _6)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _6),_yIO_ESC b e(_6) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _7)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _7),_yIO_ESC b e(_7) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _8)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _8),_yIO_ESC b e(_8) _yIO_ESC a)_yIO_IFBA62A_IN(_yIO_ESC _9)(_yIO_JFB62TE_DP(b,a,t,_yIO_ESC _9),_yIO_ESC b e(_9) _yIO_ESC a)
#define _yIO_JFB62TE_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yIO_JFB62TE_##N
#define _yIO_JFB62TE(before, after, then, else, ...) _yIO_JFB62TE_N(__VA_ARGS__,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0)(before, after, then, else, __VA_ARGS__)

/**
 * Return the argument count of arguments passed to it
 */
#define _yIO_ARGSCOUNT_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,N,...)  N
#define _yIO_ARGSCOUNT(...)  _yIO_ARGSCOUNT_N(__VA_ARGS__,96,95,94,93,92,91,90,89,88,87,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)

/**
 * Sizeof, but decay string literal into a pointer
 */
#define _yIO_SIZEOFDECAY(a)  sizeof(((void)0,a))


#define _yIO_SIZEOF_DEREF(a)  sizeof(*a)

/**
 * You know what it does. If you don't, you shouldn't be looking at this.
 */
#define _yIO_STRINGIFY(x)  #x

/**
 * Join STRingify Foreach Except First
 * Apply stringify to each argument except the first
 * Gosh these names are getting longer...
 */
#define _yIO_JSTRFEF_CB(b, a, x) _yIO_ESC b #x _yIO_ESC a
#define _yIO_JSTRFEF_0(...)
#define _yIO_JSTRFEF_2(b, a, _1, ...)  _yIO_JAPPLYFOREACH(b, a, _yIO_JSTRFEF_CB, __VA_ARGS__)
#define _yIO_JSTRFEF_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yIO_JSTRFEF_##N
#define _yIO_JSTRFEF(b, a,  ...) _yIO_JSTRFEF_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0)(b, a, __VA_ARGS__)

/**
 * Join Sizeof Decay Foreach Except First
 * Apply sizeof to each argument except the first
 * Gosh these names are getting longer...
 */
#define _yIO_JSDFEF_0(...)
#define _yIO_JSDFEF_2(b, a, _1, ...) _yIO_ESC b _yIO_APPLYFOREACHCOMMA(_yIO_SIZEOFDECAY, __VA_ARGS__) _yIO_ESC a
#define _yIO_JSDFEF_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yIO_JSDFEF_##N
#define _yIO_JSDFEF(b, a,  ...) _yIO_JSDFEF_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0)(b, a, __VA_ARGS__)

/**
 * Join Sizeof Foreach Except First
 * Apply sizeof to each argument except the first
 */
#define _yIO_JSFEF_0(...)
#define _yIO_JSFEF_2(b, a, _1, ...) _yIO_ESC b _yIO_APPLYFOREACHCOMMA(sizeof, __VA_ARGS__) _yIO_ESC a
#define _yIO_JSFEF_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yIO_JSDFEF_##N
#define _yIO_JSFEF(b, a,  ...) _yIO_JSDFEF_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0)(b, a, __VA_ARGS__)


/**
 * Join Check If First is Special Func Type
 * For callback functions just check the function has proper type
 */
#define _yIO_JCIFSFT_1(b, a, f)       _yIO_ESC b _Generic((f), _yIO_scanfunc_t: (f), _yIO_printfunc_t: (f)) _yIO_ESC a
#define _yIO_JCIFSFT_2(b, a, f, ...)  _yIO_ESC b _Generic((f), _yIO_scanfunc_t: (f), _yIO_printfunc_t: (f)) _yIO_ESC a
#define _yIO_JCIFSFT_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...)  _yIO_JCIFSFT_##N
#define _yIO_JCIFSFT(b, a, ...)  _yIO_JCIFSFT_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0)(b, a, __VA_ARGS__)


/**
 * Join IGNORE 1 first argument
 */
#define _yIO_JIGNORE1_0(...)
#define _yIO_JIGNORE1_2(b, a, _1, ...)  _yIO_ESC b __VA_ARGS__ _yIO_ESC a
#define _yIO_JIGNORE1_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yIO_JIGNORE1_##N
#define _yIO_JIGNORE1(b, a, ...) _yIO_JIGNORE1_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0)(b, a, __VA_ARGS__)

/**
 * Statically checks if a 'type' is not a type that undergoes implicit promotions.
 * So 'type' must not be char, short or float, must be anything else.
 * There are two _Generic here, because compilers strangely handle signed char and char.
 */
#define _yIO_IS_PROMOTED_TYPE(type) (\
		(void)_Generic((*(type[1]){0}),\
				signed char:0,unsigned char:0,\
				signed short:0,unsigned short:0,\
				float:0,type:0), \
		(void)_Generic((*(type[1]){0}),char:0,short:0,type:0)\
)


static inline
int _yIO_CAST_TO_INT(int v) {
	return v;
}

/**
 * Print the arguments using a custom callback function.
 * Actually we could name it: "register" function to print those arguments.
 * void yiocb(int callback(yio_scanctx_t *), ...);
 * void yiocb(int callback(yio_printctx_t *), ...);
 * @param callback The callback to call.
 * @param ... Additional arguments to call.
 */
#define yiocb(callback, ...)  ((callback, ##__VA_ARGS__),_yIO_61COMMAS)