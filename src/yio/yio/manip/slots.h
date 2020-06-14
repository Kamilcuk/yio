/**
 * @file slots.h
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config();

m4_applyforloopdefine(100, m4_SLOTS_END, `m4_dnl;
/**
 * @def YIO_PRINT_SLOT_$1()
 *
 * User define-able macro that can be used to provide
 * an user overload for specific types for scanning.
 */
#ifndef YIO_PRINT_SLOT_$1
#define YIO_PRINT_SLOT_$1()
#endif
/**
 * @def YIO_SCAN_SLOT_$1()
 *
 * User define-able macro that can be used to provide
 * an user overload for specific types for scanning.
 */
#ifndef YIO_SCAN_SLOT_$1
#define YIO_SCAN_SLOT_$1()
#endif
~)m4_dnl;


/**
 * @def _yIO_PRINT_FUNC_GENERIC_SLOTS()
 * Expanded all slots macros.
 */
#define _yIO_PRINT_FUNC_GENERIC_SLOTS() \
		m4_applyforloopdefine(100, m4_SLOTS_END, `YIO_PRINT_SLOT_$1()~)

/**
 * @def _yIO_SCAN_FUNC_GENERIC_SLOTS()
 * Expanded all slots macros.
 */
#define _yIO_SCAN_FUNC_GENERIC_SLOTS() \
		m4_applyforloopdefine(100, m4_SLOTS_END, `YIO_SCAN_SLOT_$1()~)


