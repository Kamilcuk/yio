/**
 * @file yio_macros_gen.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_include(lib.m4)m4_lib_header()
#pragma once

/**
 * @def _yIO_GENERIC_GEN_0(type, value, type, value, ...)
 * Transform each pair into \c type:value suitable for _Generic call.
 * So it results in \b type:value,type:value,...
 */
#define _yIO_GENERIC_GEN_0()
#define _yIO_GENERIC_GEN_2(_1a,_1b)          _1a:_1b
#define _yIO_GENERIC_GEN_4(_1a,_1b,_2a,_2b)  _1a:_1b,_2a:_2b
m4_forloopX(6, m4_MLVLS, `m4_ifelse(m4_eval(X % 2), 0, `
#define _yIO_GENERIC_GEN_`'X`'(m4_forloopY(1, m4_eval(X / 2), `_`'Y`'a,_`'Y`'b', `,'))  `\'
		m4_forloopY(1, m4_eval(X / 2), `_`'Y`'a:_`'Y`'b', `,')')')
m4_dnl()';
#define _yIO_GENERIC_GEN_N(m4_seqdashcomma(1,m4_MLVLS),N,...)  \
		_yIO_GENERIC_GEN_##N
#define _yIO_GENERIC_GEN(...)  \
		_yIO_GENERIC_GEN_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS,0))(__VA_ARGS__)

/**
 * @def _yIO_APPLYFOREACH(callback, args...)
 * Apply a function for each argument.
 */
#define _yIO_APPLYFOREACH_0(f)
#define _yIO_APPLYFOREACH_1(f, _1)     f(_1)
#define _yIO_APPLYFOREACH_2(f, _1,_2)  f(_1)f(_2)
m4_forloopX(3, m4_MLVLS,
		``#define _yIO_APPLYFOREACH_'X`(f, 'm4_seqdashcomma(1,X)`)'  `\'
		m4_forloopX(1, X, ``f(_'X`)'', `')'
)m4_dnl()
#define _yIO_APPLYFOREACH_N(m4_seqdashcomma(0,m4_MLVLS),N,...)  \
		_yIO_APPLYFOREACH_##N
#define _yIO_APPLYFOREACH(...)  \
		_yIO_APPLYFOREACH_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS,0))(__VA_ARGS__)

/**
 * @def _yIO_APPLYFOREACHCOMMA(callback, args...)
 * Apply a function for each argument with a comma as a separator
 */
#define _yIO_APPLYFOREACHCOMMA_0(f)
#define _yIO_APPLYFOREACHCOMMA_1(f, _1)     f(_1)
#define _yIO_APPLYFOREACHCOMMA_2(f, _1,_2)  f(_1),f(_2)
m4_forloopX(3, m4_MLVLS,
		``#define _yIO_APPLYFOREACHCOMMA_'X`(f, 'm4_seqdashcomma(1,X)`)'  `\'
		m4_forloopX(1, X, ``f(_'X`)'', `,')'
)m4_dnl()
#define _yIO_APPLYFOREACHCOMMA_N(m4_seqdashcomma(0, m4_MLVLS),N,...)  \
		_yIO_APPLYFOREACHCOMMA_##N
#define _yIO_APPLYFOREACHCOMMA(...)  \
		_yIO_APPLYFOREACHCOMMA_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS, 0))(__VA_ARGS__)

/**
 * @def _yIO_APPLYFOREACHSEP(callback, separator, args...)
 * Apply a function for each argument with a separator
 */
#define _yIO_APPLYFOREACHSEP_0(f, s)
#define _yIO_APPLYFOREACHSEP_1(f, s, _1)     f(_1)
#define _yIO_APPLYFOREACHSEP_2(f, s, _1,_2)  f(_1)s f(_2)
m4_forloopX(3, m4_MLVLS,
		``#define _yIO_APPLYFOREACHSEP_'X`(f, s, 'm4_seqdashcomma(1,X)`)'  `\'
		m4_forloopX(1, X, ``f(_'X`)'', `s ')'
)m4_dnl()
#define _yIO_APPLYFOREACHSEP_N(m4_seqdashcomma(0, m4_MLVLS),N,...)  \
		_yIO_APPLYFOREACHSEP_##N
#define _yIO_APPLYFOREACHSEP(callback, ...)  \
		_yIO_APPLYFOREACHSEP_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS, 0))(callback, __VA_ARGS__)

/**
 * @def _yIO_JAPPLYFOREACH(before, after, function, args...)
 * _yIO_JAPPLYFOREACH(before, after, function, args...)
 * Same as APPLYFOREACH but join operators 'after' and 'before' are carried over.
 *
 * Similar to that _yIO_JFB62TE but without handling 62 arguments crap.
 *
 *
 * For each of the arguments the function(before, after, arg) is executed.
 *
 * @param before Anything
 * @param after Anything too
 * @param ... function A macro function to substitute for each argument
 * @param ... Argument list
 */
#define _yIO_JAPPLYFOREACH_0(b, a, f)
#define _yIO_JAPPLYFOREACH_1(b, a, f, _1)     f(b, a, _1)
#define _yIO_JAPPLYFOREACH_2(b, a, f, _1,_2)  f(b, a, _1)f(b, a, _2)
m4_forloopX(3, m4_MLVLS,
		``#define _yIO_JAPPLYFOREACH_'X`(b, a, f, 'm4_seqdashcomma(1,X)`)'  `\'
		m4_forloopX(1, X, ``f(b, a, _'X`)'')'
)m4_dnl()
#define _yIO_JAPPLYFOREACH_N(m4_seqdashcomma(0, m4_MLVLS),N,...)  \
		_yIO_JAPPLYFOREACH_##N
#define _yIO_JAPPLYFOREACH(before, after, ...)  \
		_yIO_JAPPLYFOREACH_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS, 0))(before, after, __VA_ARGS__)

/**
 * @def _yIO_APPLYFOREACHUNPACK(callback, args...)
 * Apply a function for each argument, but the arguments need to be a pack.
 *
 *     Example:
 *        #define DOIT(NAME, TYPE)  TYPE f_##NAME(TYPE a) { return a; }
 *        #define LIST() (int, int), (uint, unsigned int)
 *        _yIO_APPLYFOREACHUNPACKCOMMA(DOIT, LIST())
 *     will output:
 *        int f_int(int a) { return a; }
 *        unsigned int f_uint(unsigned int a) { return a; }
 */
#define _yIO_APPLYFOREACHUNPACK_0(f)
#define _yIO_APPLYFOREACHUNPACK_1(f, _1)     f _1
#define _yIO_APPLYFOREACHUNPACK_2(f, _1,_2)  f _1 f _2
m4_forloopX(3, m4_MLVLS,
		``#define _yIO_APPLYFOREACHUNPACK_'X`(f, 'm4_seqdashcomma(1,X)`)'  `\'
		m4_forloopX(1, X, ``f _'X` '' `')'
)m4_dnl()
#define _yIO_APPLYFOREACHUNPACK_N(m4_seqdashcomma(0,m4_MLVLS),N,...)  \
		_yIO_APPLYFOREACHUNPACK_##N
#define _yIO_APPLYFOREACHUNPACK(...)  \
		_yIO_APPLYFOREACHUNPACK_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS,0))(__VA_ARGS__)

/**
 * @def _yIO_APPLYFOREACHUNPACKCOMMA(callback, args...)
 * Apply a function for each argument, but the arguments need to be inside braces
 * and join them with a comma. *
 * @param ... callback to call foreach argument
 * @param ... arguments to unpack from braces to call
 */
#define _yIO_APPLYFOREACHUNPACKCOMMA_0(f)
#define _yIO_APPLYFOREACHUNPACKCOMMA_1(f, _1)     f _1
#define _yIO_APPLYFOREACHUNPACKCOMMA_2(f, _1,_2)  f _1,f _2
m4_forloopX(3, m4_MLVLS,
		``#define _yIO_APPLYFOREACHUNPACKCOMMA_'X`(f, 'm4_seqdashcomma(1,X)`)'  `\'
		m4_forloopX(1, X, ``f _'X`'', `,')'
)m4_dnl()
#define _yIO_APPLYFOREACHUNPACKCOMMA_N(m4_seqdashcomma(0,m4_MLVLS),N,...)  \
		_yIO_APPLYFOREACHUNPACKCOMMA_##N
#define _yIO_APPLYFOREACHUNPACKCOMMA(...)  \
		_yIO_APPLYFOREACHUNPACKCOMMA_N(__VA_ARGS__,m4_seqcomma(m4_MLVLS,0))(__VA_ARGS__)


/**
 * _yIO_STRLEN(string) - unfolded string checking
 */
#define _yIO_STRLEN(s) (\
		!(s)[0]?0:!(s)[1]?1:\
		m4_forloopX(2, 120,``!(s)['X`]?'X`:'m4_ifelse(m4_eval(X%5),`1',`\
		')') \
		-1)

m4_forloopX(100, m4_SLOTS_END, `
#ifndef YIO_PRINT_SLOT_`'X
#define YIO_PRINT_SLOT_`'X`'()
#endif

#ifndef YIO_SCAN_SLOT_`'X
#define YIO_SCAN_SLOT_`'X`'()
#endif
')


#define _yIO_PRINT_SLOTS()  m4_forloopX(100, m4_SLOTS_END, ` YIO_PRINT_SLOT_`'X`'()')m4_dnl';

#define _yIO_SCAN_SLOTS()   m4_forloopX(100, m4_SLOTS_END, ` YIO_SCAN_SLOT_`'X`'()')m4_dnl';


