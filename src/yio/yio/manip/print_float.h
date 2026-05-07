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

#include "../../private/yio_float.h"
#include "../../private/yio_string.h"
#include "../ctx.h"

/* Layer 2: Representation Dispatchers */
{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_REPR_{{R}}
int YYIO_float_dispatch_{{R}}(yio_printctx_t *t, YYIO_FLOAT_REPR_{{R}} val);
#endif
{% endfor %}

{% for V in j_FLOATS %}
#line
#if YIO_HAS_FLOAT{{V.1}}

{% if V.1 in ["f", "d", "l"] %}
#line
int YYIO_print_{{V.1}}(yio_printctx_t *t);
#define YYIO_PRINT_FLOAT{{V.1}} YYIO_print_{{V.1}}
{% else %}
#line
{% for R in V.reprs %}
#if YYIO_REPR_OF_{{V.1}}_IS_{{R}}
int YYIO_print_{{V.1}}_as_{{R}}(yio_printctx_t *t);
#define YYIO_PRINT_FLOAT{{V.1}} YYIO_print_{{V.1}}_as_{{R}}
#endif
{% endfor %}
{% endif %}

#if defined(YYIO_PRINT_FLOAT{{V.1}})
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
