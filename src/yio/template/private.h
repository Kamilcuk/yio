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
#include <string.h>
#include <limits.h>
#if _yIO_TYPE_YWIO
#include <wctype.h>
#elif _yIO_TYPE_YUIO
#include <unictype.h>
#include <uniconv.h>
#include <unistr.h>
#include <uniwidth.h>
#endif
