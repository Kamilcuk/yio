m4_divert(-1)

# m4_forloop(iterator, start_range, end_range, substitution, separator)
# Generates iterators from start_range to end_range substitution
# the iterator in substitution string separating substitutiong
# by the separator.
# Example:
#    m4_forloop(`I', 1, 4, `m4_forloop(`J', 1, I, `(I, J)')')
# will output:
#    (1, 1)(2, 1)(2, 2)(3, 1)(3, 2)(3, 3)(4, 1)(4, 2)(4, 3)(4, 4)
m4_define(`m4_forloop',
	`m4_ifelse(
		m4_eval(`($2) <= ($3)'),
		`1',
		`m4_pushdef(`$1')m4__forloop(
			m4_eval(`$2'),
			m4_eval(`$3'),
			`m4_define(`$1',',
			`)$4',
			`$5',
			`m4_incr')m4_popdef(`$1')',
		`m4_ifelse(
			m4_eval(`($2) >= ($3)'),
			`1',
			`m4_pushdef(`$1')m4__forloop(
				m4_eval(`$2'),
				m4_eval(`$3'),
				`m4_define(`$1',',
				`)$4',
				`$5',
				`m4_decr')m4_popdef(`$1')',
			`'m4_dnl
)'m4_dnl
)'m4_dnl
)

# m4__forloop(iterator, end, define_new_ierator, body_new_iterator, incr/decr)
# Internal function for m4_forloop
m4_define(`m4__forloop',
	`$3`$1'$4`'m4_ifelse(
		`$1',
		`$2',
		`',
		`$5$0($6(`$1'),`$2',`$3',`$4',`$5',`$6')'m4_dnl
)'m4_dnl
)


# m4_forloopI(start_range, end_range, substitution_with_I, separator)
# m4_forloop shortcut to use with `I` as separator
m4_define(`m4_forloopI',`m4_forloop(`I',`$1',`$2',`$3',`$4')')

# m4_forloopI(start_range, end_range, substitution_with_I, separator)
# m4_forloop shortcut to use with `X` as separator
m4_define(`m4_forloopX',`m4_forloop(`X',`$1',`$2',`$3',`$4')')

# m4_forloopY(start_range, end_range, substitution_with_Y, separator)
# m4_forloop shortcut to use with `Y` as separator
m4_define(`m4_forloopY',`m4_forloop(`Y',`$1',`$2',`$3',`$4')')

m4_divert(0)m4_dnl