m4_divert(`-1')
# upcase(text)
# downcase(text)
# capitalize(text)
#   change case of text, improved version
m4_define(`m4_upcase', `m4_translit(`$*', `a-z', `A-Z')')
m4_define(`m4_downcase', `m4_translit(`$*', `A-Z', `a-z')')
m4_define(`_arg1', `$1')
m4_define(`_to_alt', `m4_changequote(`<<[', `]>>')')
m4_define(`_from_alt', `m4_changequote(<<[`]>>, <<[']>>)')
m4_define(`_upcase_alt', `m4_translit(<<[$*]>>, <<[a-z]>>, <<[A-Z]>>)')
m4_define(`_downcase_alt', `m4_translit(<<[$*]>>, <<[A-Z]>>, <<[a-z]>>)')
m4_define(`_capitalize_alt',
  `regexp(<<[$1]>>, <<[^\(\w\)\(\w*\)]>>,
    <<[_upcase_alt(<<[<<[\1]>>]>>)_downcase_alt(<<[<<[\2]>>]>>)]>>)')
m4_define(`capitalize',
  `_arg1(_to_alt()patsubst(<<[<<[$*]>>]>>, <<[\w+]>>,
    _from_alt()`]>>_$0_alt(<<[\&]>>)<<['_to_alt())_from_alt())')
m4_divert`'m4_dnl