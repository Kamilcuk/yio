#!/bin/sh
echo "$1"
echo "$2" | sort | 
   sed 's/[[:space:]]*\([^ ]*\) - /\1~/' |
   if hash column 2>/dev/null; then
       if column --version 2>&1 | grep -q util-linux; then
            column -t -s "~" -o "  - ";
       else
           column -t -s "~";
       fi;
   else
      sed 's/|/ - /';
   fi |
   sed 's/^/  /'

