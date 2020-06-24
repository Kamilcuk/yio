
/**
 * @def m4_filename
 * @ingroup m4
 * @return The filename from __file__
 */
#define m4_filename(...)
m4_define(`m4_filename~, `m4_patsubst(m4___file__, `^.*\(/\|\\\)~)~)

/**
 * @def m4_resultname
 * @ingroup m4
 * @return The filename with m4 suffix.
 */
#define m4_resultname(...)
m4_define(`m4_resultname~, `m4_patsubst(m4_filename, `\.m4$~)~)

/**
 * @def m4_lib_header()
 * @ingroup m4
 * This library header to be included as the first line of a file.
 */
#define m4_lib_header(...)
m4_define(`m4_lib_header~, `// DO NOT EDIT THIS FILE!!!
// This file was autogenerated by m4___program__
// From m4___file__
// Edit the m4 source file to edit this file.
#if 0
#include "m4___file__"
#endif
m4_dnl Add m4_lib_header_suffix if it is defined and nonempty.
m4_ifdef(`m4_lib_header_suffix~, `m4_ifelse(m4_lib_header_suffix, `~, `~, `m4_lib_header_suffix
~)~)m4_dnl
#line m4_eval(m4___line__ + 2) "m4___file__"
m4_dnl ~)

/**
 * @def m4_config(TYPE)
 * @ingroup m4
 * @param token Type of configuration to load.
 * @brief Loads configuration from proper cfg_*.m4 file.
 */
#define m4_config(...)
m4_define(`m4_config~, `_m4_config_in(m4_ifelse(`$1~, `~, `yio~, `$1~))~)
m4_define(`_m4_config_in~, `m4_ifdef(
		`YTYPE~,
		`~,
		`// Loading cfg_$1.m4
m4_include(`cfg_$1.m4~)
m4_lib_header($1)~)~)~)
/**
 * Configures the file to use normal strings configuration.
 */
#define m4_config_yio()
m4_define_function(`m4_config_yio~, `m4_config(`yio~)~)m4_dnl;

/**
 * Configures the file to use wide strings configuration.
 */
#define m4_config_ywio()
m4_define_function(`m4_config_ywio~, `m4_config(`ywio~)~)m4_dnl;

/**
 * Configures the file to use utf-8 strings configuration.
 */
#define m4_config_yuio()
m4_define_function(`m4_config_yuio~, `m4_config(`yuio~)~)m4_dnl;
