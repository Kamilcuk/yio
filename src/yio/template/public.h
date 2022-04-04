/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
{% if MODEX == 2 %}
#include <wchar.h>
{% elif MODEX == 3 %}
#include <uchar.h>
#include <stdint.h>
#if _yIO_HAS_UNISTRING
#include <unitypes.h>
#endif
{% endif %}
#include "fmt.h"
#include "ctx.h"
#include "io.h"
#include "manip/manip.h"
#include "yio_macros.h"
