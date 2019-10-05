/*
 * yioopt.c
 *
 *  Created on: 4 paŸ 2019
 *      Author: Kamil
 */
#include "yio.h"

int _yyy_yioopt_set_flag(yioctx_t *t) {
	yioctx_opt_nonconst(t)->flags |= yioctx_va_arg(t, enum yioflags_e);
	return yioctx_next(t);
}

int _yyy_yioopt_clr_flag(yioctx_t *t) {
	yioctx_opt_nonconst(t)->flags &= yioctx_va_arg(t, enum yioflags_e);
	return yioctx_next(t);
}

int _yyy_yioopt_precision(yioctx_t *t) {
	yioctx_opt_nonconst(t)->precision = yioctx_va_arg(t, int);
	return yioctx_next(t);
}
