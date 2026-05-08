/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_FLOAT_H_
#define YYIO_YIO_YIO_MANIP_PRINT_FLOAT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

{% for V in j_FLOATS %}
#line
#if YIO_HAS_FLOAT{{V.1}}

#line
int YYIO_print_{{V.1}}(yio_printctx_t *t);
#define YYIO_PRINT_FLOAT{{V.1}} YYIO_print_{{V.1}}

#ifdef __cplusplus
{% if V.1 in ["f", "d", "l"] %}
#define YYIO_PRINT_FUNC_GENERIC_FLOAT{{V.1}}()  \
		YYIO_OVERLOAD_TYPE_FUNC(YYIO_FLOAT{{V.1}}, YYIO_PRINT_FLOAT{{V.1}})
{% else %}
#define YYIO_PRINT_FUNC_GENERIC_FLOAT{{V.1}}()
{% endif %}
#else
#define YYIO_PRINT_FUNC_GENERIC_FLOAT{{V.1}}()  \
		YYIO_OVERLOAD_TYPE_FUNC(YYIO_FLOAT{{V.1}}, YYIO_PRINT_FLOAT{{V.1}})
#endif

#else

#define YYIO_PRINT_FUNC_GENERIC_FLOAT{{V.1}}()

#endif

{% endfor %}

#define YYIO_PRINT_FUNC_GENERIC_FLOATS() \
		{% for V in j_FLOATS %}
		YYIO_PRINT_FUNC_GENERIC_FLOAT{{V.1}}() \
		{% endfor %}
		/**/

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_FLOAT_H_
