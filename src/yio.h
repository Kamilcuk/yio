/*
 * yio.h
 *
 *  Created on: 27 wrz 2019
 *      Author: Kamil
 */

#ifndef YIO_H_
#define YIO_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>

/**
 * Namespaces:
 * _yyy_* - very private
 * _yio* - to be used by implementators implementing their own print/scan callbacks
 * yio_* - public input output manipulators
 * ypf* - printf format specifiers
 * ysf* - scanf format specifiers
 */

#ifdef __CDT_PARSER__
#define _Generic(a, ...)   ((void)/*_Generic*/(a))
#endif

/* Private Macro Library -------------------------------------------------------------------------------- */

/**
 * Re-evaulate the arguments
 */
#define _yyy_ESC(...) __VA_ARGS__

/**
 * Ignore first argument
 */
#define _yyy_IGNORE1_IN(_1,...)  __VA_ARGS__
#define _yyy_IGNORE1(...)         _yyy_IGNORE1_IN(__VA_ARGS__,0)

/**
 * Commas
 */
#define _yyy_58COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define _yyy_59COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define _yyy_60COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define _yyy_61COMMAS ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

/**
 * _yyy_IFBA62A
 * If braces and 62 or more arguments then 'then' else 'else'
 */
#define _yyy_IFBA62A_0(then, else)   else
#define _yyy_IFBA62A_62(then, else)  then
#define _yyy_IFBA62A_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,N,...) _yyy_IFBA62A_##N
#define _yyy_IFBA62A_IN(expr) _yyy_IFBA62A_N(expr,62,62,62,62,62,62,62,62,62,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define _yyy_IFBA62A(expr, then, else)  _yyy_IFBA62A_IN(_yyy_ESC _1)(then, else)

/**
 * _yyy_JFB62TE
 * Join for each argument iF braces with 62 arguments or more apply 'Then' else apply 'Else' also pass separators along
 *
 * This function is basically the hearth of everything that happens here.
 * The 'before' and 'after' are separators that are used to join the list. First two arguments should be inside braces.
 * The 'then' function may choose to not expand before nor after, if the function sees it fit. The should be expanded with _yyy_ESC.
 * Special care needs to be taken to conform to ISO C99, that at least one argument needs to go to ellipsis.
 *
 * Running:
 *   #define FUNC_ON_62_ARGS(before, after, arg1, arg2, ...) _yyy_ESC before RESULT(arg1, arg2, __VA_ARGS__) _yyy_ESC after
 *   #define FUNC_ON_LESS_THEN_62_ARGS(arg)  RESULT(arg)
 *   _yyy_JFB62TE((BEFORE), (AFTER), FUNC_ON_62_ARGS, FUNC_ON_LESS_THEN_62_ARGS, ARG1, ARG2, ((ARG3), _yyy_60COMMAS), ((ARG4, ARG5, ARG6), _yyy_60COMMAS))
 * runs:
 *   - foreach argument
 *     - if that argument doesn't has 62 arguments
 *        - do then(before, after, UNESCAPE(GET_FIRST_ARGUMENT(UNESCAPE(argment))) )
 *     - else
 *        - do _yyy_ESC before then(argument) _yyy_ESC after
 *
 */
#define _yyy_JFB62TE_DP_IN(b,a,t,_1,...) t(b,a,_1)
#define _yyy_JFB62TE_DP(b,a,t,args) _yyy_JFB62TE_DP_IN(b,a,t,_yyy_ESC args)
#define _yyy_JFB62TE_1(b,a,t,e,_1)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)
#define _yyy_JFB62TE_2(b,a,t,e,_1,_2)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)
#define _yyy_JFB62TE_3(b,a,t,e,_1,_2,_3)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)
#define _yyy_JFB62TE_4(b,a,t,e,_1,_2,_3,_4)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _4)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _4),_yyy_ESC b e(_4) _yyy_ESC a)
#define _yyy_JFB62TE_5(b,a,t,e,_1,_2,_3,_4,_5)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _4)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _4),_yyy_ESC b e(_4) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _5)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _5),_yyy_ESC b e(_5) _yyy_ESC a)
#define _yyy_JFB62TE_6(b,a,t,e,_1,_2,_3,_4,_5,_6)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _4)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _4),_yyy_ESC b e(_4) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _5)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _5),_yyy_ESC b e(_5) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _6)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _6),_yyy_ESC b e(_6) _yyy_ESC a)
#define _yyy_JFB62TE_7(b,a,t,e,_1,_2,_3,_4,_5,_6,_7)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _4)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _4),_yyy_ESC b e(_4) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _5)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _5),_yyy_ESC b e(_5) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _6)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _6),_yyy_ESC b e(_6) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _7)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _7),_yyy_ESC b e(_7) _yyy_ESC a)
#define _yyy_JFB62TE_8(b,a,t,e,_1,_2,_3,_4,_5,_6,_7,_8)_yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _4)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _4),_yyy_ESC b e(_4) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _5)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _5),_yyy_ESC b e(_5) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _6)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _6),_yyy_ESC b e(_6) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _7)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _7),_yyy_ESC b e(_7) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _8)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _8),_yyy_ESC b e(_8) _yyy_ESC a)
#define _yyy_JFB62TE_9(b,a,t,e,_1,_2,_3,_4,_5,_6,_7,_8,_9)  _yyy_IFBA62A_IN(_yyy_ESC _1)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _1),_yyy_ESC b e(_1) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _2)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _2),_yyy_ESC b e(_2) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _3)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _3),_yyy_ESC b e(_3) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _4)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _4),_yyy_ESC b e(_4) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _5)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _5),_yyy_ESC b e(_5) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _6)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _6),_yyy_ESC b e(_6) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _7)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _7),_yyy_ESC b e(_7) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _8)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _8),_yyy_ESC b e(_8) _yyy_ESC a)_yyy_IFBA62A_IN(_yyy_ESC _9)(_yyy_JFB62TE_DP(b,a,t,_yyy_ESC _9),_yyy_ESC b e(_9) _yyy_ESC a)
#define _yyy_JFB62TE_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yyy_JFB62TE_##N
#define _yyy_JFB62TE(before, after, then, else, ...) _yyy_JFB62TE_N(__VA_ARGS__,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0)(before, after, then, else, __VA_ARGS__)

/**
 * Return the argument count of arguments passed to it
 */
#define _yyy_ARGSCOUNT_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,N,...)  N
#define _yyy_ARGSCOUNT(...)  _yyy_ARGSCOUNT_N(__VA_ARGS__,96,95,94,93,92,91,90,89,88,87,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)

/**
 * Apply a function for each argument
 */
#define _yyy_APPLYFOREACH_0(a)
#define _yyy_APPLYFOREACH_1(a, _1)  a(_1)
#define _yyy_APPLYFOREACH_2(a, _1,_2) a(_1), a(_2)
#define _yyy_APPLYFOREACH_3(a, _1,_2,_3)  a(_1), a(_2), a(_3)
#define _yyy_APPLYFOREACH_4(a, _1,_2,_3,_4)  a(_1), a(_2), a(_3), a(_4)
#define _yyy_APPLYFOREACH_5(a, _1,_2,_3,_4,_5)  a(_1), a(_2), a(_3), a(_4), a(_5)
#define _yyy_APPLYFOREACH_6(a, _1,_2,_3,_4,_5,_6)  a(_1), a(_2), a(_3), a(_4), a(_5), a(_6)
#define _yyy_APPLYFOREACH_7(a, _1,_2,_3,_4,_5,_6,_7)  a(_1), a(_2), a(_3), a(_4), a(_5), a(_6), a(_7)
#define _yyy_APPLYFOREACH_8(a, _1,_2,_3,_4,_5,_6,_7,_8)  a(_1), a(_2), a(_3), a(_4), a(_5), a(_6), a(_7), a(_8)
#define _yyy_APPLYFOREACH_9(a, _1,_2,_3,_4,_5,_6,_7,_8,_9)  a(_1), a(_2), a(_3), a(_4), a(_5), a(_6), a(_7), a(_8), a(_9)
#define _yyy_APPLYFOREACH_10(a, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10)  a(_1), a(_2), a(_3), a(_4), a(_5), a(_6), a(_7), a(_8), a(_9), a(_10)
#define _yyy_APPLYFOREACH_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yyy_APPLYFOREACH_##N
#define _yyy_APPLYFOREACH(...) _yyy_APPLYFOREACH_N(__VA_ARGS__,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)(__VA_ARGS__)

/**
 * Sizeof, but decay string literal into a pointer
 */
#define _yyy_SIZEOFDECAY(a)  sizeof(((void)0,a))

/**
 * Join Sizeof Decay Foreach Except First
 * Apply sizeof to each argument except the first
 * Gosh these names are getting longer...
 */
#define _yyy_JSDFEF_0(...)
#define _yyy_JSDFEF_2(b, a, _1, ...) _yyy_ESC b _yyy_APPLYFOREACH(_yyy_SIZEOFDECAY, __VA_ARGS__) _yyy_ESC a
#define _yyy_JSDFEF_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yyy_JSDFEF_##N
#define _yyy_JSDFEF(b, a,  ...) _yyy_JSDFEF_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0)(b, a, __VA_ARGS__)


/**
 * Join Check If First is Special Func Type
 * For callback functions just check the function has proper type
 */
#define _yyy_JCIFSFT_1(b, a, f)       _yyy_ESC b _Generic((f), _yyy_func_t: (f)) _yyy_ESC a
#define _yyy_JCIFSFT_2(b, a, f, ...)  _yyy_ESC b _Generic((f), _yyy_func_t: (f)) _yyy_ESC a
#define _yyy_JCIFSFT_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...)  _yyy_JCIFSFT_##N
#define _yyy_JCIFSFT(b, a, ...)  _yyy_JCIFSFT_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0)(b, a, __VA_ARGS__)


/**
 * Join IGNORE 1 first argument
 */
#define _yyy_JIGNORE1_0(...)
#define _yyy_JIGNORE1_2(b, a, _1, ...)  _yyy_ESC b __VA_ARGS__ _yyy_ESC a
#define _yyy_JIGNORE1_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yyy_JIGNORE1_##N
#define _yyy_JIGNORE1(b, a, ...) _yyy_JIGNORE1_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0)(b, a, __VA_ARGS__)

/**
 * Checks if a 'type' is not a type that undergoes implicit promotions.
 * So 'type' must not be char, short or float, must be anything else.
 * There are two _Generic here, because compilers strangely handle signed char and char.
 */
#define _yyy_IS_PROMOTED_TYPE(type) (\
		(void)_Generic((*(type[1]){0}),\
				signed char:0,unsigned char:0,\
				signed short:0,unsigned short:0,\
				float:0,type:0), \
		(void)_Generic((*(type[1]){0}),char:0,short:0,type:0)\
)

/* Private Macro ------------------------------------------------------------------------------------ */

/**
 * For one argument choose the printing function dynamically using _Generic macro
 */
#define _yyy_PRINT_FUNC_GENERIC(arg)  _Generic((arg), \
		char: _yyy_print_char, \
		signed char: _yyy_print_schar, \
		unsigned char: _yyy_print_uchar, \
		short: _yyy_print_short, \
		unsigned short: _yyy_print_ushort, \
		int: _yyy_print_int, \
		unsigned int: _yyy_print_uint, \
		long: _yyy_print_long, \
		unsigned long: _yyy_print_ulong, \
		long long: _yyy_print_llong, \
		unsigned long long: _yyy_print_ullong, \
		float: _yyy_print_float, \
		double: _yyy_print_double, \
		long double: _yyy_print_ldouble, \
		char *: _yyy_print_charpnt, \
		const char *: _yyy_print_constcharpnt, \
		wchar_t *: _yyy_print_wcharpnt, \
		const wchar_t *: _yyy_print_constwcharpnt, \
		default: _Generic((arg), \
		wchar_t: _yyy_print_wchar, \
		int *: _yyy_print_intpnt, \
		float *: _yyy_print_floatpnt, \
		default: _yyy_print_wrong_type \
		))

/**
 * Choose the scanning function of argument using _Generic macro
 */
#define _yyy_SCAN_FUNC_GENERIC(arg)  _Generic((arg), \
		short *: _yyy_scan_short, \
		unsigned short *: _yyy_scan_ushort, \
		int *: _yyy_scan_int, \
		unsigned int *: _yyy_scan_uint, \
		long *: _yyy_scan_int, \
		unsigned long *: _yyy_scan_uint, \
		long long *: _yyy_scan_int, \
		unsigned long long *: _yyy_scan_uint, \
		float *: _yyy_scan_float, \
		double *: _yyy_scan_double, \
		long double *: _yyy_scan_ldouble, \
		char *: _yyy_scan_char, \
		char **: _yyy_scan_charpntpnt, \
		)

/**
 * _yyy_yprint_arguments
 * Create argument list for yprint functions
 * If an argument doesn't has 62 commas, then the function for it is chosen
 *    using _Generic expression with _yyy_PRINT_FUNC_GENERIC.
 * If an argument does has 62 commas, then the first argument that should be inside braces is extracted
 *   and the first argument from inside the braces is the function to handle the arguments.
 * The arguments are appended to the end of the function.
 *
 * For example a call like:
 *    yprint(1, _yio_cb(function, 2, 3));
 * Is expanded to:
 *    yprint((const _yyy_func_t[]){ _Generic((1), int: _yyy_print_int), function, NULL }, 1, 2, 3);
 *
 * The first argument is an array of functions to handle arguments, delimetered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define _yyy_yprint_arguments_1(fmt)  (fmt), NULL
#define _yyy_yprint_arguments_2(fmt,...) \
	(fmt), \
	&(const struct _yyy_yio_data_s){ \
		(const _yyy_func_t []){ \
			_yyy_JFB62TE((), (,), _yyy_JCIFSFT, _yyy_PRINT_FUNC_GENERIC, __VA_ARGS__) \
			NULL \
		}, \
		(const size_t[]){ \
			_yyy_JFB62TE((), (,), _yyy_JSDFEF, _yyy_SIZEOFDECAY, __VA_ARGS__) \
			0 \
		}, \
	} \
	_yyy_JFB62TE((,), (), _yyy_JIGNORE1, _yyy_ESC, __VA_ARGS__)
#define _yyy_yprint_arguments_N(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,N,...) _yyy_yprint_arguments_##N
#define _yyy_yprint_arguments(...) _yyy_yprint_arguments_N(__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0)(__VA_ARGS__)


/**
 * _yio_cb
 * Print the arguments using a custom callback function.
 * Actually we could name it: "register" function to print those arguments.
 * It is with underscore as it is semi-private.
 */
// static inline void _yio_cb(int callback(yioctx_t *), ...);
#define _yio_cb(...)  ((__VA_ARGS__),_yyy_60COMMAS)
#define _ypfcb _yio_cb

// static inline void yio_fmt(const char fmt[], int _optional_a, int _optional_b);
// TODO: overload and check arguments type with _Generic
#define _yyy_yiofmt_0() _Pragma("ERROR: yiofmt needs an argument")
#define _yyy_yiofmt_1(fmt) _Generic((fmt),char*:(fmt),const char*:(fmt))
#define _yyy_yiofmt_2(fmt, spec) _Generic((fmt),char*:(fmt),const char*:(fmt)), _Generic((spec),int:(spec))
#define _yyy_yiofmt_3(fmt, spec, spec2) _Generic((fmt),char*:(fmt),const char*:(fmt)), _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yyy_yiofmt_N(_0,_1,_2,N,...) _yyy_yiofmt_##N
#define yiofmt(...) _yio_cb(_yyy_yio_fmt, _yyy_yiofmt_N(__VA_ARGS__,3,2,1,0)(__VA_ARGS__))
#define ypf yiofmt

/* Private Types ------------------------------------------------------------------------------------------- */

/* Exported Types -------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------- */

/**
 * Flags that can be set as configuration of yio stream
 * 1:1 rip-off from C++, see C++ for documentation for meanings
 */
enum yioflags_e {
	YIO_BOOLALPHA   = 1 << 0,
	YIO_SHOWBASE    = 1 << 1,
	YIO_SHOWPOINT   = 1 << 2,
	YIO_SHOWPOS     = 1 << 3,
	YIO_SKIPWS      = 1 << 4,
	YIO_UNITBUF     = 1 << 5,
	YIO_UPPERCASE   = 1 << 6,
	YIO_FIXED       = 1 << 7,
	YIO_SCIENTIFIC  = 1 << 8,
	YIO_INTERNAL    = 1 << 9,
	YIO_LEFT        = 1 << 10,
	YIO_RIGHT       = 1 << 11,
	YIO_HEX         = 1 << 12,
	YIO_OCTAL       = 1 << 13,
	YIO_PERSISTENT  = 1 << 14,
	YIO_FLAGS_END   = 1 << 15,
	YIO_HEXFLOAT    = YIO_FIXED | YIO_SCIENTIFIC,
};

/* ---------------------------------------------------------------- */

/**
 * Represents input/output configuration.
 */
struct yioopt_s {
	/**
	 * User custom specifier
	 */
	union {
		int flags;
		void *pnt;
	} custom;
	/**
	 * Bitmask of flags as above
	 */
	unsigned flags;
	/**
	 * The width field from printf
	 */
	int width;
	/**
	 * The precision field from printf
	 */
	int precision;
	/**
	 * The fill character, can be a space or zero as of now.
	 */
	char fill;
};

__attribute__((__nonnull__))
static inline
void yioopt_init(struct yioopt_s *t) {
	*t = (const struct yioopt_s){
		.flags = YIO_UNITBUF,
		.precision = -1,
		.width = -1,
	};
}

/* ---------------------------------------------------------------- */

typedef struct _yioctx_s yioctx_t;

/**
 * The type of callback functions.
 * It is expected(!) that the first thing the function will do is read the arguments from va_list
 * Even if function fails, the `*va` pointer has to be updated with the arguments read.
 * This is the most important contract.
 * @param out the FILE* stream to scan or print to
 * @param flags the yio flags to format the printing with
 * @param va A _pointer_ to va_list, to read the arguments from.
 * @return -1 on error and 0 on success.
 */
typedef int _yyy_plain_func_t(yioctx_t *t);
typedef _yyy_plain_func_t *_yyy_func_t;

/**
 * Get's passed to generic functions as first argument
 */
struct _yyy_yio_data_s {
	/**
	 * Initialized with valid list of _yyy_func pointers
	 */
	const _yyy_func_t * const funcs;
	/**
	 * Initialized with list of argument sizes for argument checking
	 */
	const size_t * const argsizes;
};

typedef const struct _yyy_yio_data_s _yio_data_t;

/* ------------------------------------------------------------------------------ */

struct _yioctx_s {
	const struct _yyy_yio_data_s *data;
	FILE *file;
	const _yyy_func_t *ifunc;
	va_list *va;
#ifndef NDEBUG
	const size_t *argsizespnt;
#endif
	struct yioopt_s opt;
	int writtencnt;
	int *itemscntcb;
};

__attribute__((__nonnull__, __warn_unused_result__, __returns_nonnull__))
static inline
const struct yioopt_s *yioctx_opt(const yioctx_t *t) {
	return &t->opt;
}

__attribute__((__nonnull__, __warn_unused_result__, __returns_nonnull__))
static inline
struct yioopt_s *yioctx_opt_nonconst(yioctx_t *t) {
	return &t->opt;
}

__attribute__((__nonnull__, __warn_unused_result__))
static inline
bool yioctx_no_file(const yioctx_t *t) {
	assert(t != NULL);
	return t->file == NULL;
}

__attribute__((__nonnull__, __warn_unused_result__))
static inline
FILE *yioctx_file(const yioctx_t *t) {
	assert(t != NULL);
	assert(t->file != NULL);
	return t->file;
}

__attribute__((__nonnull__, __warn_unused_result__))
static inline
_yyy_func_t _yyy_yioctx_get_next_func(yioctx_t *t) {
	assert(t != NULL);
	assert(t->ifunc != NULL);
	return *t->ifunc++;
}

__attribute__((__nonnull__, __warn_unused_result__))
static inline
int yioctx_next(yioctx_t *t) {
	return _yyy_yioctx_get_next_func(t)(t);
}

__attribute__((__nonnull__))
static inline
void _yyy_yioctx_argsizespnt_inc(yioctx_t *t) {
	t->argsizespnt++;
}

#define _yyy_yioctx_va_arg_in(yioctx, type) ( \
	assert((yioctx) != NULL), \
	assert((yioctx)->data != NULL), \
	assert(*(yioctx)->argsizespnt != 0 && !!"You called va_arg too many times"), \
	assert(*(yioctx)->argsizespnt == sizeof(type) && !!"You called va_arg with invalid type"), \
	_yyy_yioctx_argsizespnt_inc(yioctx) \
)

__attribute__((__nonnull__, __warn_unused_result__))
static inline
va_list *_yyy_yioctx_va_list(const yioctx_t *t) {
	return t->va;
}

/**
 * Get next argument from variadic arguments stack. The argument has type `type`.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that `type` is not a promoted type, see _yyy_IS_PROMOTED_TYPE
 */
#define yioctx_va_arg(yioctx, type)  yioctx_va_arg2(yioctx, type, type)

/**
 * Get next argument of type 'realtype' from arguments stack.
 * The real argument type after implicit conversion is 'promotedtype'.
 * This function has only usage for 'realtype' equal to char, short or float.
 */
#define yioctx_va_arg2(yioctx, realtype, promotedtype)  (\
		_yyy_IS_PROMOTED_TYPE(promotedtype), \
		_yyy_yioctx_va_arg_in(yioctx, realtype), \
		va_arg(*_yyy_yioctx_va_list(yioctx), promotedtype)\
)

/* Private Functions ---------------------------------------------------------------------------------------- */

int _yyy_yioopt_set_flag(yioctx_t *t);
int _yyy_yioopt_clr_flag(yioctx_t *t);
int _yyy_yioopt_precision(yioctx_t *t);

const char *_yyy_yfunc_to_string_in(const void *func);

int _yyy_yio_fmt(yioctx_t *t);

/* ------------------------------------------------------------------- */

/* ------------------------------------------------------------------- */

int _yyy_print_char(yioctx_t *t);
int _yyy_print_schar(yioctx_t *t);
int _yyy_print_uchar(yioctx_t *t);
int _yyy_print_short(yioctx_t *t);
int _yyy_print_ushort(yioctx_t *t);
int _yyy_print_int(yioctx_t *t);
int _yyy_print_uint(yioctx_t *t);
int _yyy_print_long(yioctx_t *t);
int _yyy_print_ulong(yioctx_t *t);
int _yyy_print_llong(yioctx_t *t);
int _yyy_print_ullong(yioctx_t *t);
int _yyy_print_charpnt(yioctx_t *t);
int _yyy_print_constcharpnt(yioctx_t *t);
int _yyy_print_intpnt(yioctx_t *t);
int _yyy_print_wchar(yioctx_t *t);
int _yyy_print_wcharpnt(yioctx_t *t);
int _yyy_print_constwcharpnt(yioctx_t *t);
int _yyy_print_float(yioctx_t *t);
int _yyy_print_double(yioctx_t *t);
int _yyy_print_ldouble(yioctx_t *t);
int _yyy_print_wrong_type(yioctx_t *t);
int _yyy_print_floatpnt(yioctx_t *t);
int _yyy_print_endl(yioctx_t *t);

int yvprint(_yio_data_t *data, va_list *va);
int yvfprint(FILE *file, _yio_data_t *data, va_list *va);
int yvsnprint(char *dest, size_t size, _yio_data_t *data, va_list *va);

int _yyy_yprintf(const char fmt[], _yio_data_t *data, ...);
int _yyy_yfprintf(FILE *file, const char fmt[], _yio_data_t *data, ...);
int _yyy_ysnprintf(char *dest, size_t size, const char fmt[], _yio_data_t *data, ...);

__attribute__((__nonnull__(1, 3)))
int yvprintf(const char fmt[], _yio_data_t *data, va_list *va);
__attribute__((__nonnull__(2, 4)))
int yvfprintf(FILE *file, const char fmt[], _yio_data_t *data, va_list *va);
__attribute__((__nonnull__(1, 3, 5)))
int yvsnprintf(char *dest, size_t size, const char fmt[], _yio_data_t *data, va_list *va);

int _yio_unittest(void);

/* Exported functions and macros ------------------------------------------------------------------------------------------- */

#define yio_right()         _ypfcb(_yyy_yioopt_set_flag, YIO_RIGHT)
#define yio_noright()       _ypfcb(_yyy_yioopt_clr_flag, YIO_RIGHT)
#define yio_fixed()         _ypfcb(_yyy_yioopt_set_flag, YIO_FIXED)
#define yio_scientific()    _ypfcb(_yyy_yioopt_set_flag, YIO_SCIENTIFIC)
#define yio_hexfloat()      _ypfcb(_yyy_yioopt_set_flag, YIO_FIXED | YIO_SCIENTIFIC)
#define yio_persistent()    _ypfcb(_yyy_yioopt_set_flag, YIO_PERSISTENT)
#define yio_nopersistent()  _ypfcb(_yyy_yioopt_clr_flag, YIO_PERSISTENT)
#define yio_precision(arg)  _ypfcb(_yyy_yioopt_precision, _Generic((arg),int:(arg)))
#define yio_width(arg)      _ypfcb(_yyy_yioopt_width, _Generic((arg),int:(arg)))
#define yio_endl()          _ypfcb(_yyy_print_endl)
#define yio_flush()         _ppfcb(_yyy_print_flush)
#define yio_unitbuf()       _ypfcb(_yyy_yioopt_set_flag, YIO_UNITBUF)
#define yio_nounitbuf()     _ypfcb(_yyy_yioopt_clr_flag, YIO_UNITBUF)

#define yprint(...)                _yyy_yprintf(_yyy_yprint_arguments("",__VA_ARGS__))
#define yfprint(file, ...)         _yyy_yfprintf(file, _yyy_yprint_arguments("",__VA_ARGS__))
#define ysnprint(dest, size, ...)  _yyy_ysnprintf(dest, size, _yyy_yprint_arguments("",__VA_ARGS__))
#define yprintf(...)                _yyy_yprintf(_yyy_yprint_arguments(__VA_ARGS__))
#define yfprintf(file, ...)         _yyy_yfprintf(file, _yyy_yprint_arguments(__VA_ARGS__))
#define ysnprintf(dest, size, ...)  _yyy_ysnprintf(dest, size, _yyy_yprint_arguments(__VA_ARGS__))

/* END ----------------------------------------------------------------------------------------------------------- */

#endif /* YIO_H_ */
