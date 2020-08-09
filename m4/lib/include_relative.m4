
`
#define m4_include_relative ~
m4_define_function(`m4_include_relative~,
`m4_include(m4_patsubst(m4___file__, `/[^/]*$~)/$1)~)

`
#define m4_include_relative_quiet ~
m4_define_function(`m4_include_relative_quiet~,
`m4_divert(-1)m4_include_relative(`$@~)m4_divert(0)~)
