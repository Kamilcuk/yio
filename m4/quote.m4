divert(`-1')
# m4_quote(args) - convert args to single-quoted string
define(`m4_quote', `ifelse(`$#', `0', `', ``$*'')')
# m4_dquote(args) - convert args to quoted list of quoted strings
define(`m4_dquote', ``$@'')
# m4_dquote_elt(args) - convert args to list of double-quoted strings
define(`m4_dquote_elt', `ifelse(`$#', `0', `', `$#', `1', ```$1''',
                             ```$1'',$0(shift($@))')')
divert`'dnl