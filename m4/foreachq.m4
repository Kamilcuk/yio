include(`quote.m4')dnl
divert(`-1')
# m4_foreachq(x, `item_1, item_2, ..., item_n', stmt)
#   quoted list, simple version
define(`m4_foreachq', `pushdef(`$1')_m4_foreachq($@)popdef(`$1')')
define(`_m4_arg1', `$1')
define(`_m4_foreachq', `ifelse(m4_quote($2), `', `',
  `define(`$1', `_m4_arg1($2)')$3`'$0(`$1', `shift($2)', `$3')')')
divert`'dnl