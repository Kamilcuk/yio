divert(`-1')
# m4_foreach(x, (item_1, item_2, ..., item_n), stmt)
#   parenthesized list, simple version
define(`m4_foreach', `pushdef(`$1')_m4_foreach($@)popdef(`$1')')
define(`_m4_arg1', `$1')
define(`_m4_foreach', `ifelse(`$2', `()', `',
  `define(`$1', _m4_arg1$2)$3`'$0(`$1', (shift$2), `$3')')')
divert`'dnl
