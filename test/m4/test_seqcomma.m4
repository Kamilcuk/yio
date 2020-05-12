m4_seqcomma(1, 5)
m4_divert(-1)
// PASS_REGULAR_EXPRESSION 1,2,3,4,5
m4_divert(0)m4_dnl
m4_seqcommaX(1, 5, `hello`~X~)
m4_divert(-1)
// PASS_REGULAR_EXPRESSION hello1,hello2,hello3,hello4,hello5
m4_divert(0)