/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#if _yIO_HAS_STDFIX_TYPES

m4_divert(-1)
// `m4_stdfix_types~
m4_include(`yio/private/yio_stdfix.h~)
m4_divert(0)

m4_applyforeachdefine((m4_stdfix_types), `m4_dnl;
/// Print $2 type variable to yπio_printctx stream
int _yΩIO_print_$3(yπio_printctx_t *t);
~)m4_dnl;

#define _yΩIO_PRINT_STDFIX() \
		m4_applyforeachdefine((m4_stdfix_types), `m4_dnl;
		$2: _yΩIO_print_$3, \
		~)

#else // _yIO_HAS_STDFIX_TYPES
#define _yΩIO_PRINT_STDFIX()
#endif // _yIO_HAS_STDFIX_TYPES

