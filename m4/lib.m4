m4_divert(-1)
m4_include(changequote_to_tilde.m4)
m4_changecom(`/*~, `*/~)

m4_sinclude(yio_config.m4)

m4_include(lib/define_function.m4)
m4_include(lib/define_name.m4)
m4_include(lib/forloop.m4)
m4_include(lib/applyforloop.m4)
m4_include(lib/applyforloopdefine.m4)
m4_include(lib/forloopdash.m4)
m4_include(lib/applyforeach.m4)
m4_include(lib/applyforeachdefine.m4)
m4_include(lib/seq.m4)
m4_include(lib/foreach.m4)
m4_include(lib/capitalize2.m4)
m4_include(m4_config.m4)

/**
 * @defgroup m4 
 * @brief Group with m4 macros.
 */

/**
 * @def dnl
 * @ingroup m4
 * @brief Define dnl just for shortcuts to m4_dnl, but please do not use it.
 */
#define dnl
m4_define(`dnl~, `m4_dnl~)

/**
 * @def m4_fatal_error(...)
 * @param ... List of any arguments to print
 * @ingroup m4
 * @brief Just print an error and exits m4 with an error.
 */
#define m4_fatal_error(...)
m4_define(`m4_fatal_error~, `m4_errprint(__file__:__line__`: error: $*
~)m4exit(`1~)~)

m4_define(`m4_forloopXcomma~, `m4_forloopX(`$1~, `$2~, `$3~, `,~)~)
m4_define(`m4_forloopYcomma~, `m4_forloopY(`$1~, `$2~, `$3~, `,~)~)
m4_define(`m4_forargs~, `m4_forloopXcomma(`$1~, `$2~, ``_~X~)~)
m4_define(`m4_fornumbers~, `m4_forloopXcomma(`$1~, `$2~, `X~)~)
m4_define(`m4_check_args_count~, `m4_ifelse(m4_eval(`($2) != ($3)~), `1~,
	`m4_fatal_error(`Wrong number of arguments passed to $1: $4~)~)~)

m4_divert(0)m4_dnl
