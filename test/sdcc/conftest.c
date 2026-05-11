#include "conftest.h"
#include <yio.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __SDCC
#if defined(__SDCC_mcs51) || defined(__SDCC_ds390) || defined(__SDCC_ds400)
static volatile YYIO_XDATA YYIO_AT(0xffff) unsigned char SIM_CONTROL;
static volatile YYIO_XDATA YYIO_AT(0x00fe) unsigned char EXIT_CODE;
#else
static volatile unsigned char SIM_CONTROL;
static volatile unsigned char EXIT_CODE;
#endif

void exit_test(int code) {
	if (code) {
		puts("\n! ERROR - exited with nonzero exit code !");
	} else {
		puts("SUCCESS");
	}
	EXIT_CODE = (unsigned char)code;
	abort();
}

static void putchar_in(char c) {
	SIM_CONTROL = 0x70;
	SIM_CONTROL = c;
}

static unsigned counter = 0;
int putchar(int c) {
	if (counter > 1000) {
		char *str = "\nFOREVER ERROR\n";
		while (*str) {
			putchar_in(*str++);
		}
		abort();
	} else {
		counter++;
	}
	putchar_in(c);
	return c;
}

void yyio_break(void) {
	__asm
	nop
	__endasm;
}

void abort(void) {
	SIM_CONTROL = 0x73;
	SIM_CONTROL = 0x00;
	yyio_break();
	while (1);
}

int libtest_main();
#undef main
int main() {
	puts("START");
	exit_test(libtest_main());
	return 0;
}
#endif
