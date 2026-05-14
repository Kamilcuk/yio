#include "conftest.h"
#include <yio.h>
#include <stdio.h>
#include <stdlib.h>

YIO_TEST_BUF_ATTR YIO_TEST_BUF_AT char yyio_test_buf[32];

#ifdef __SDCC
#if defined(__SDCC_ds390)
#include <ds80c390.h>
#endif
#if defined(__SDCC_mcs51) || defined(__SDCC_ds390) || defined(__SDCC_ds400)
static volatile YIO_XDATA YIO_AT(0xffff) unsigned char SIM_CONTROL;
static volatile YIO_XDATA YIO_AT(0x00fe) unsigned char EXIT_CODE;
#elif defined(__SDCC_stm8)
static volatile YIO_AT(0x5800) unsigned char SIM_CONTROL;
static volatile unsigned char EXIT_CODE;
#else
static volatile unsigned char SIM_CONTROL;
static volatile unsigned char EXIT_CODE;
#endif

void abort(void) {
	SIM_CONTROL = 0x73;
	SIM_CONTROL = 0x00;
	while (1);
}

static void putchar_in(char c) {
	SIM_CONTROL = 0x70;
	SIM_CONTROL = c;
}

static void print_str_in(const char *s) {
	while (*s) putchar_in(*s++);
}

static unsigned counter = 0;
int putchar(int c) {
	if (counter > 1000) {
		print_str_in("\nFOREVER ERROR\n");
		abort();
	} else {
		counter++;
	}
	putchar_in(c);
	return c;
}

static void print_str(const char *s) {
	while (*s) putchar(*s++);
}

static void print_uint8(unsigned char n) {
	char buf[4];
	char *p = &buf[3];
	*p = '\0';
	if (n == 0) {
		*--p = '0';
	} else {
		while (n > 0) {
			*--p = (n % 10) + '0';
			n /= 10;
		}
	}
	print_str(p);
}

void exit_test(int code) {
	print_str("\nEXIT: ");
	print_uint8((unsigned char)code);
	putchar('\n');
	EXIT_CODE = (unsigned char)code;
	abort();
}

int libtest_main();
#undef main
int main() {
	puts("START");
	exit_test(libtest_main());
	return 0;
}
#endif
