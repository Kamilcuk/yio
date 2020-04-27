m4_include(lib.m4)
m4_divert(-1)
/**
 * @file cfg_yio.m4
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */

m4_define(`m4_lib_header_suffix', `#include <wchar.h>')
m4_define(`YIO', `_yWIO_$@')
m4_define(`Yio', `ywio_$@')
m4_define(`Ychar', `wchar_t')
m4_define(`Yint', `wint_t')
m4_define(`YEOF', `WEOF')
m4_define(`Ychar_constant', `L$1')
m4_define(`Ystrlen', `wcslen($@)')

m4_divert()m4_dnl