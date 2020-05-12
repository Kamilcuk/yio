m4_forloopdashX(1, 5, ``hello~X~, `, ~)
m4_divert(-1)
// PASS_REGULAR_EXPRESSION hello_1, hello_2, hello_3, hello_4, hello_5
m4_divert(0)