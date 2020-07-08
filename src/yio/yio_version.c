/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_version.h"

unsigned long yio_get_version(void) {
	return YIO_VERSION;
}

const char *yio_get_version_string(void) {
	return YIO_VERSION_STRING;
}

