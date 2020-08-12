
// Order matters
m4_include(lib/define_function.m4)
m4_include(lib/define_name.m4)

// Include all files excluding this file and the one above
m4_esyscmd(`
		find ~m4_patsubst(m4___file__, `/lib.m4*$~)` -type f \
			-name '*.m4' \
			'!' -name 'lib.m4' \
			'!' -name 'define_function.m4' \
			'!' -name 'define_name.m4' \
			-printf 'm4_include(lib/%f)\n'
~)
