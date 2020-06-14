
/**
 * @def m4_define_blind(name, [value])
 * 
 * https://www.gnu.org/software/m4/manual/m4-1.4.14/html_node/Composition.html
 */
m4_define(`m4_define_blind~, `m4_ifelse(`$#~, `0~, ``$0~~,
	`_$0(`$1~, `$2~, `$~`#~, `$~`0~)~)~)
m4_define(`_m4_define_blind~, `m4_define(`$1~,
	`m4_ifelse(`$3~, `0~, ``$4~~, `$2~)~)~)

m4_define(`m4_define_func~, `m4_define_blind($@)~)
