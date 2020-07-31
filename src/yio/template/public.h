/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_ifdef(`_yIO_TYPE_YWIO~, `#include <wchar.h>~)
m4_ifdef(`_yIO_TYPE_YUIO~, `#include <unitypes.h>~)
#include "fmt.h"
#include "ctx.h"
#include "io.h"
#include "manip/manip.h"
#include "yio_macros.h"
