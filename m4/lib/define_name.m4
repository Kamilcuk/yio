/**
 * @def m4_define_name(name, value)
 * @param name Macro name
 * @param value Expansion value.
 * @brief Defines macro to replace preserving arguments.
 *
 * Defines _name_ as a only name macro, such that
 * _name_ will expand to value and preserve all arguments
 */
m4_define(`m4_define_name~, `m4_ifelse(`$#~, `0~, ``$0~~,
	`_$0(`$1~, `$2~, `$~`#~, `$~`@~)~)~)
m4_define(`_m4_define_name~, `m4_define(`$1~,
	``$2~m4_ifelse(`$3~, `0~, `~, `($4)~)~)~)
	