/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#define _yΩIO_PRIVATE  1
#include "public.h"
#include "io.h"
#include "ctx_private.h"
#include "commonctx_private.h"
#include "fmt_private.h"
#include "../yio_private.h"
#include "../private/yio_strlib.h"
#include <string.h>
#include <limits.h>
{% if MODEX == 2 %}
#include <wctype.h>
{% elif MODEX == 3 %}
#include <unictype.h>
#include <uniconv.h>
#include <unistr.h>
#include <uniwidth.h>
#include <unistdio.h>
{% endif %}
