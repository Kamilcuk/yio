/**
 * @file
 * @date 2020-08-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_syncline(1)m4_dnl;
#include <yio/yio_config.h>
#ifdef _yIO_HAS_strfromf
#define YIO_PRINT_FLOATS_WITH YIO_PRINT_FLOATS_WITH_STRFROM
#include "test_print_float_on_strto.c"
#else
#include <yπio.h>
int main() {
	return yπprint("");
}
#endif
