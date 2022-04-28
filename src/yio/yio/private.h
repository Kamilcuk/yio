/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "public.h"
#include "../yio_private.h"
#include "../private/yio_strlib.h"
#include <string.h>
#include <limits.h>
#if TMODEX == 2
#include <wctype.h>
#elif TMODEX == 3
#include <unictype.h>
#include <uniconv.h>
#include <unistr.h>
#include <uniwidth.h>
#include <unistdio.h>
#endif
