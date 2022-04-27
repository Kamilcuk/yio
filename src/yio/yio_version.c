/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_version.h"

static const char banner[] =
"YIO Library "YIO_VERSION_STRING" compiled on "__DATE__" "__TIME__"\n"
"Copyright (C) 2020 by Kamil Cukrowski\n"
"This is free software; see the source for copying conditions.  There is NO\n"
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
;

unsigned long yio_get_version(void) {
	return YIO_VERSION;
}

const char *yio_get_version_string(void) {
	return YIO_VERSION_STRING;
}

const char *yio_get_version_banner(void) {
	return banner;
}

