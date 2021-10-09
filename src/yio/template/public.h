/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_template_chooser2(«», «m4_dnl);
#include <wchar.h>
»,«m4_dnl;
#include <uchar.h>
#include <stdint.h>
#if _yIO_HAS_UNISTRING
#include <unitypes.h>
#endif
»)m4_dnl;
#include "fmt.h"
#include "ctx.h"
#include "io.h"
#include "manip/manip.h"
#include "yio_macros.h"
