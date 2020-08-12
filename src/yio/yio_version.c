/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_version.h"

static const char yio_version_string[] = YIO_VERSION_STRING;

static const char yio_banner[] = "\
YIO Library "YIO_VERSION_STRING" compiled on "__DATE__" "__TIME__"\n\
Copyright (C) 2020 by Kamil Cukrowski\n\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
";

int yio_print_version_banner(FILE *f) {
	return fwrite(yio_banner, sizeof(yio_banner) - 1, 1, f) == 1 ? 0 : -1;
}

unsigned long yio_get_version(void) {
	return YIO_VERSION;
}

const char *yio_get_version_string(void) {
	return yio_version_string;
}

