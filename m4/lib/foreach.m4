
/**
 * m4_foreach(x, (item_1, item_2, ..., item_n), stmt)
 *
 * https://www.gnu.org/software/m4/manual/m4-1.4.14/html_node/Foreach.html
 */
#define m4_foreach(...)
m4_define(`m4_foreach~, `m4_pushdef(`$1~)_m4_foreach($@)m4_popdef(`$1~)~)
m4_define(`_m4_foreach_arg1~, `$1~)
m4_define(`_m4_foreach~, `m4_ifelse(`$2~, `()~, `~,
  `m4_define_name(`$1~, _m4_foreach_arg1$2)$3`~$0(`$1~, (m4_shift$2), `$3~)~)~)

