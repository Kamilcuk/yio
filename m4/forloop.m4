divert(`-1')

# m4_forloop(iterator, start_range, end_range, substitution, separator)
# Generates iterators from start_range to end_range substitution
# the iterator in substitution string separating substitutiong 
# by the separator.
# Example:
#    m4_forloop(`I', 1, 4, `m4_forloop(`J', 1, I, `(I, J)')')
# will output:  
#    (1, 1)(2, 1)(2, 2)(3, 1)(3, 2)(3, 3)(4, 1)(4, 2)(4, 3)(4, 4)
define(`m4_forloop',
	`ifelse(
		eval(`($2) <= ($3)'),
		`1',
		`pushdef(`$1')m4__forloop(
			eval(`$2'),
			eval(`$3'),
			`define(`$1',',
			`)$4',
			`$5',
			`incr')popdef(`$1')',
		`ifelse(
			eval(`($2) >= ($3)'),
			`1',
			`pushdef(`$1')m4__forloop(
				eval(`$2'),
				eval(`$3'),
				`define(`$1',',
				`)$4',
				`$5',
				`decr')popdef(`$1')',
			`'dnl
)'dnl
)'dnl
)

# m4__forloop(iterator, end, define_new_ierator, body_new_iterator, incr/decr)
# Internal function for m4_forloop
define(`m4__forloop',
	`$3`$1'$4`'ifelse(
		`$1',
		`$2',
		`',
		`$5$0($6(`$1'),`$2',`$3',`$4',`$5',`$6')'dnl
)'dnl
)


# m4_forloopI(start_range, end_range, substitution_with_I, separator)
# m4_forloop shortcut to use with `I` as separator
define(`m4_forloopI',`m4_forloop(`I',`$1',`$2',`$3',`$4')')

# m4_forloopI(start_range, end_range, substitution_with_I, separator)
# m4_forloop shortcut to use with `X` as separator
define(`m4_forloopX',`m4_forloop(`X',`$1',`$2',`$3',`$4')')

# m4_forloopY(start_range, end_range, substitution_with_Y, separator)
# m4_forloop shortcut to use with `Y` as separator
define(`m4_forloopY',`m4_forloop(`Y',`$1',`$2',`$3',`$4')')

divert`'dnl