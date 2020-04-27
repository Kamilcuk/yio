m4_include(lib.m4)
m4_divert(-1)
/**
 * @file cfg_yio.m4
 * @author Kamil Cukrowski
 * @date 25.05.2020
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */

m4_define(`m4_lib_header_suffix', `')
m4_define(`YIO', `_yIO_$@')
m4_define(`Yio', `yio_$@')
m4_define(`Ychar', `char')
m4_define(`Yint', `int')
m4_define(`YEOF', `EOF')
m4_define(`Ychar_constant', `$1')
m4_define(`Ystrlen', `strlen($@)')

m4_divert()m4_dnl