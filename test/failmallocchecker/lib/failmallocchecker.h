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
		for (int OK_VAR = (fmc_init(__FILE__, __LINE__, __func__), 1); \
			fmc_continue(); fmc_next(), OK_VAR = 0)

#define FMC_IGNORE() \
		for (int _tOdO = (fmc_ignore(), true); _tOdO; _tOdO = (fmc_noignore(), false))

void fmc_init(const char *file, int line, const char *func);
int fmc_continue(void);
void fmc_next(void);
void fmc_print(void);
void fmc_ignore(void);
void fmc_noignore(void);

#endif /* TEST_FAILMALLOCCHECKER_FAILMALLOCCHECKER_H_ */
