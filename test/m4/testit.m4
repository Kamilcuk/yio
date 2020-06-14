m4_divert(-1)

m4_define(`m4_TEST~, `m4_divert(1)$1
m4_divert(-1)~)

m4_include(m4_TESTFILE)

m4_divert(0)
