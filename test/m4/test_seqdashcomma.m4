m4_seqdashcomma(1, 5)
m4_divert(-1)
// PASS_REGULAR_EXPRESSION _1,_2,_3,_4,_5
m4_divert(0)m4_dnl
m4_seqdashcommaX(1, 5, `hello`~X~)
m4_divert(-1)
// PASS_REGULAR_EXPRESSION hello_1,hello_2,hello_3,hello_4,hello_5
m4_divert(0)