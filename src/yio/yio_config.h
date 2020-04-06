/**
 * @file yio_config.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once

#include "gen/yio_config_gen.h"

/**
 * Set to 1 if compiling with -pedantic flag.
 */
#ifndef _yIO_COMPILES_PEDANTIC
#define _yIO_COMPILES_PEDANTIC  1
#endif

/**
 * Enable compilation of format specifiers that use:
 * - _FloatN
 * - _FloatNx
 */
#ifndef _yIO_USE__FLOATN
#define _yIO_USE__FLOATN  !_yIO_COMPILES_PEDANTIC
#endif

/**
 * Enable compilation of format specifiers that use:
 * - _DecimalN
 * - _DecimalNx
 */
#ifndef _yIO_USE__DECIMALN
#define _yIO_USE__DECIMALN  !_yIO_COMPILES_PEDANTIC
#endif

/**
 * Enable compilation of int128_t type
 */
#ifndef _yIO_USE___INT128
#define _yIO_USE___INT128  _yIO_HAVE_INT128_T
#endif




