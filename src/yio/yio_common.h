/**
 * @file common.h
 * @date 21 kwi 2020
 * @author Kamil Cukrowski
 * @copyright SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifdef __CDT_PARSER__

/**
 * @defgroup privatemacros
 * @brief Private macros for internal use on m4 preprocessing stage
 * These symbols should be defined by m4/cfg_*.m4 files.
 * @{
 */
/**
 * @def YIO(NAME)
 * @param NAME  Symbol name
 * Declares the symbol within private yio namespace.
 */
#define YIO(NAME)  _yIO_ ## NAME
/**
 * @def Yio(NAME)
 * @param NAME  Symbol name
 * Declares the symbol within private yio namespace
 */
#define Yio(NAME)  yio_ ## NAME
/**
 * @def Ychar
 * Represents a character. A pointer to it represents a string.
 */
#define Ychar  char
/**
 * @def Yint
 * A character or EOF.
 */
#define Yint   int
/**
 * @def YEOF
 * The EOF character.
 */
#define YEOF   EOF
/**
 * @def Ychar_constant(x)
 * @param x A character within single qoutes.
 * Defines a character constant.
 */
#define Ychar_constant(x)  x
/**
 * @def Ystrlen(str)
 * @param str <tt>const Ychar*</tt> A zero terminated string.
 * Get's the count of characters (not bytes) in a string.
 */
#define Ystrlen(STR)       strlen(STR)
/**
 * @}
 */

#endif // __CDT_PARSER__
