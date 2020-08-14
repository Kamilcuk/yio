/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio_config_gen.h"

/**
 * @defgroup config Configuration group generated from cmake.
 * @{
 */
 /**
 * Internal variable set used to check if config is sourced.
 */
#define _yIO_HAS_CONFIG  error["Do not expand this macro"]
/**
 * @}
 */

#ifndef _yIO_HAS_UNISTD_H
#error _yIO_HAS_UNISTD_H
#endif

// check
#if !_yIO_HAS_UNISTD_H
#if YIO_USE_OUTPUT_FD
#error "You can't output to fd without unistd.h"
#endif
#if YIO_USE_INPUT_FD
#error "You can't read from fd without unistd.h"
#endif
#endif


