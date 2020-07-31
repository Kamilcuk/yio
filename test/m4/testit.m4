m4_divert(-1)

m4_dnl Note - this adds additional line
m4_define(`m4_TEST~, `m4_divert(1)$1
m4_divert(-1)~)

m4_dnl We are changing the comment to `#` "back".
m4_changecom(`#~, `~)

m4_include(m4_TESTFILE)

m4_divert(0)
