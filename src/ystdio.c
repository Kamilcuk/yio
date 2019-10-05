#include <stdio.h>
#include <stdlib.h>
#include "yio.h"

int main(void) {
	fprintf(stderr, "!!!Hello World!!!\n"); /* prints !!!Hello World!!! */
	_yio_unittest();
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
