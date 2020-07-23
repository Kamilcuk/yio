/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <stdlib.h>
#include <failmallocchecker.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define test(expr)  do{ \
		if(!(expr)) { \
			fprintf(stderr, "%s:%d: %s\n", __func__, __LINE__, #expr); \
			abort(); \
		} \
	}while(0)

int lib() {
	void *a = malloc(1);
	void *b = calloc(1, 2);
	void *c = realloc(a, 3);
	if (c == NULL) free(a);
	free(b);
	free(c);
	ssize_t s = write(STDOUT_FILENO, (char[1]){'0' + (a == NULL)}, 1);
	test(s == 1);
	s = write(STDOUT_FILENO, (char[1]){'0' + (b == NULL)}, 1);
	test(s == 1);
	s = write(STDOUT_FILENO, (char[1]){'0' + (c == NULL)}, 1);
	test(s == 1);
	s = write(STDOUT_FILENO, (char[1]){'\n'}, 1);
	test(s == 1);
	return
			((a == NULL) ? 0x01 : 0x00) |
			((b == NULL) ? 0x02 : 0x00) |
			((c == NULL) ? 0x04 : 0x00) ;
}

void test_me() {
	int vars[4];
	int i = 0;
	FMC_FOR(ok) {
		vars[i] = lib();
		if (ok) {
			test(vars[i] == 0);
			test(i == 0);
		} else {
			test(vars[i] != 0);
			test(i != 0);
		}
		i++;
	}
	test(i == 4);
	test(vars[0] == 0x00);
	test(vars[1] == 0x01);
	test(vars[2] == 0x02);
	test(vars[3] == 0x04);
}

int main() {
	test_me();
	test_me();
	test_me();
}
