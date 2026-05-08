/**
 * @file
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRIVATE_H_
#define YYIO_YIO_YIO_MANIP_PRIVATE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../private.h"
#include "manip.h"
#include "../../private/yio_string.h"

int YYIO_float_apply_alternate_form(yio_printctx_t *t, YYIO_string *o);

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRIVATE_H_
