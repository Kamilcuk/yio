/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#if _yIO_HAS_STDFIX_TYPES

{% from "yio/private/yio_stdfix.h" import j_STDFIX %}

{% call j_FOREACHAPPLY(j_STDFIX) %}
/// Print $2 type variable to yπio_printctx stream
int _yΩIO_print_$3(yπio_printctx_t *t);
{% endcall %}

#define _yΩIO_PRINT_STDFIX() \
		{% call j_FOREACHAPPLY(j_STDFIX) %}
		$2: _yΩIO_print_$3, \
		{% endcall %}
		/**/

#else // _yIO_HAS_STDFIX_TYPES
#define _yΩIO_PRINT_STDFIX()
#endif // _yIO_HAS_STDFIX_TYPES

