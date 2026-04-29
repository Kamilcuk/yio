/**
 * @file yio_test.c
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_test.h"

#if __arm__ && __GNUC__
extern void initialise_monitor_handles(void);
__attribute__((__constructor__))
static void init() {
    initialise_monitor_handles();
}
#endif

#ifdef __SDCC
int putchar(int c) {
	*((volatile char __xdata *)0xffff) = 0x70;
	*((volatile char __xdata *)0xffff) = (char)c;
	return c;
}
int puts(const char *s) {
    while (*s) {
        putchar(*s++);
    }
    putchar('\n');
}
void exit_test(int code) {
	if (code) {
		puts("\n\n! ERROR - exited with nonzero exit code !\n\n");
	} else {
		*((volatile char __xdata *)0xffff) = 0x73;
	}
	abort();
	while (1);
}
void abort(void) {
	*((volatile char __xdata *)0xffff) = 0x61;
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
