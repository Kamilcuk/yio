divert(`-1')

# m4_seqcomma(start_number, end_numbers)
# generates numbers with separated by a comm
# example:  
#   m4_loopdashcomma(1, 5)
# outputs:
#   1,2,3,4,5
define(`m4_seqcomma',`m4_forloopX(`$1', `$2', `X', `,')')

# m4_seqdashcomma(start_number, end_numbers)
# generates numbers with a leading dash separated by a comm
# example:  
#   m4_loopdashcomma(1, 5)
# outputs:
#   _1,_2,_3,_4,_5
define(`m4_seqdashcomma',`m4_forloopX(`$1', `$2', ``_'X', `,')')

divert`'dnl