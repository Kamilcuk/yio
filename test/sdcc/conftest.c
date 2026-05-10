#include "conftest.h"
#include <yio.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __SDCC
static volatile __xdata __at(0xffff) unsigned char SIM_CONTROL;

void exit_test(int code) {
	if (code) {
		puts("\n! ERROR - exited with nonzero exit code !");
	} else {
		puts("SUCCESS");
	}
	abort();
}

static void putchar_in(char c) {
	SIM_CONTROL = 0x70;
	SIM_CONTROL = c;
}

static unsigned counter = 0;
int putchar(int c) {
	if (counter > 200) {
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

void abort(void) {
	SIM_CONTROL = 0x73;
	SIM_CONTROL = 0x00;
	__asm
	.db 0x45 ; Illegal opcode/Breakpoint in some simulators
	__endasm;
	while (1);
}

int libtest_main();
#undef main
int main() {
	exit_test(libtest_main());
	return 0;
}
#endif
