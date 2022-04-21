/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_DEBUG_H_
#define YIO_YIO_DEBUG_H_
#include "yio.h"

#if __GNUC__
#  define _yIO_DEBUG_FUNCTION	__extension__ __PRETTY_FUNCTION__
#else
# if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#  define _yIO_DEBUG_FUNCTION	__func__
# else
#  define _yIO_DEBUG_FUNCTION	((const char *) 0)
# endif
#endif

{% for I in j_range(j_MLVLS) %}
#define _yIO_ydebug_{{I}}({{j_seqdashcomma(I)}}) \
	{% for J in j_range(I) %}
		#_{{J}}"=", _{{J}}{% if not loop.last %}, " ", \
{% endif %}
	{% endfor %}

{% endfor %}
#define _yIO_ydebug({{j_seqdashcomma(j_MLVLS)}}, N, ...)  \
		_yIO_ydebug_##N


#define ydebug(...) \
	yfprint(stderr, \
			__FILE__, (char)':', __LINE__, (char)':', _yIO_DEBUG_FUNCTION, (char)':', (char)' ', \
			_yIO_ydebug(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})(__VA_ARGS__), (char)'\n')
#define ydebugil(...)  \
	yfprint(stderr, \
			_yIO_ydebug(__VA_ARGS__, {{j_seqcomma(j_MLVLS, 0)}})(__VA_ARGS__), (char)'\n')

#endif
