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
#endif /* _yIO_YIO_YΩIO_PUBLIC_H_ */
