/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_CACHE_AUTO_H_
#define _yIO_YIO_PRIVATE_YIO_CACHE_AUTO_H_

/**
 * The default buffer size allocated on stack
 * for buffering output things so that we do not use dynamic
 * allocation when the size is smaller then this size.
 */
#define YIO_CACHE_STACK_SIZE 32

#if ! defined YIO_CACHE_STACK_SIZE || YIO_CACHE_STACK_SIZE < 0
#error "YIO_CACHE_STACK_SIZE not defined"
#endif

/**
 * @def _yIO_CACHE_AUTO_DECL
 * Initialize 3 variables for use with _yIO_res.
 * Uses global macro YIO_CACHE_STACK_SIZE
 * @param bufvar Name of the buffer variable.
 * @param resultvar Name of the result variable.
 * @param lengthvar Name of the returned length variable.
 */
/**
 * @def _yIO_CACHE_AUTO_FREE
 * Frees the allocated resources when necesseraly.
 * @param bufvar @see _yIO_CACHE_AUTO_DECL
 * @param resultvar @see _yIO_CACHE_AUTO_DECL
 * @param lengthvar @see _yIO_CACHE_AUTO_DECL
 */
#if YIO_CACHE_STACK_SIZE > 0
#define _yIO_CACHE_AUTO_DECL(bufvar, resultvar, lengthvar)  \
		char bufvar[YIO_CACHE_STACK_SIZE], *resultvar = bufvar; \
		size_t lengthvar = sizeof(bufvar)
#define _yIO_CACHE_AUTO_FREE(bufvar, resultvar, lengthvar)  \
		do{ if(resultvar != bufvar) free(resultvar); }while(0)
#else
#define _yIO_CACHE_AUTO_DECL(bufvar, resultvar, lengthvar) \
		char *resultvar = NULL; \
		size_t lengthvar = 0
#define _yIO_CACHE_AUTO_FREE(bufvar, resultvar, lengthvar)  \
		do{}while(0)
#endif
#endif /* _yIO_YIO_PRIVATE_YIO_CACHE_AUTO_H_ */
