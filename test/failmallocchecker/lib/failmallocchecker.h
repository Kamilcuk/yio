/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef TEST_FAILMALLOCCHECKER_FAILMALLOCCHECKER_H_
#define TEST_FAILMALLOCCHECKER_FAILMALLOCCHECKER_H_

/* configuration --------------------------------------------- */

#define FMC_METHOD_GLIBC_MALLOC_HOOK  1
#define FMC_METHOD_WRAP_MALLOC        2
#define FMC_METHOD_CALL___LIBC        3

#ifndef FMC_USE_METHOD
#define FMC_USE_METHOD  FMC_METHOD_GLIBC_MALLOC_HOOK
#endif

#ifndef FMC_DISABLE
#define FMC_DISABLE  0
#endif

/* ------------------------------------------------------------ */

#if FMC_DISABLE

#define FMC_FOR(OK_VAR) \
	for(int OK_VAR = 1, _tOdO = 1; _tOdO; _tOdO = 0)
#define FMC_IGNORE_BLOCK(OK_VAR) \
	for(int _tOdO = 1; _tOdO; _tOdO = 0)

#else

/**
 * Iterate over each malloc failing.
 * This for will execute the inner statement:
 * - the first time counting the count of malloc statements.
 * - then foreach malloc, it will make the malloc fail
 * The "OK_VAR" is a variable that will be set to nonzero if the
 * code is "ok" - ie. all malloc will succeed. On the consecutive runs,
 * "OK_VAR" will be zero meaning that one of the malloc will fail.
 */
#define FMC_FOR(OK_VAR) \
		for (int OK_VAR = ( \
				_fmc_init(__FILE__, __LINE__, __func__), \
				1); \
				_fmc_continue(); \
				_fmc_next(), \
				OK_VAR = 0)

/**
 * Ignore malloc trapping inside the block.
 */
#define FMC_IGNORE_BLOCK() \
		for (int _tOdO = (_fmc_ignore(), 1); _tOdO; _fmc_noignore(), _tOdO = 0)

void _fmc_init(const char *file, int line, const char *func);
int _fmc_continue(void);
void _fmc_next(void);
void _fmc_print(void);
void _fmc_ignore(void);
void _fmc_noignore(void);

#endif

#endif /* TEST_FAILMALLOCCHECKER_FAILMALLOCCHECKER_H_ */
