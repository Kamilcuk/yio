/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_PUBLIC_H_
#define _yIO_YIO_YΩIO_PUBLIC_H_
#include "../yio_public.h"
#if TMODEX == 2
#include <wchar.h>
#elif TMODEX == 3
#include <uchar.h>
#endif
#include "ctx.h"
#include "io.h"
#include "manip/manip.h"
#include "yio_print_arguments.h"
#endif /* _yIO_YIO_YΩIO_PUBLIC_H_ */
